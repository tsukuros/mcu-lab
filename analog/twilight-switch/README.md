# twilight-switch

A dusk-to-dawn switch on an ESP32-S3. A photoresistor in a voltage divider is
read by the ADC, the dark/light decision is made with **hysteresis**, and the
load is switched by a relay driven through a transistor.

## Circuit

```
+3.3V ──[ LDR ]──┬── GPIO 4 (ADC1_CH3)
                 │
 GND  ──[ 10k ]──┘

              +5V ──[ 10k ]──┬── IN (relay module, SRD-05VDC-SL-C)
                             │ C
GPIO 15 ──[ 10k ]────────── B ┤ BC547B
                             │ E
              GND ───────────┴── common ground for both supplies
```

**Two inversions cancel out.** The common-emitter stage inverts, and the relay
module is active-LOW (verified by measurement). Together: `HIGH` on GPIO 15
means the relay is **on**. That's why there is no `!` anywhere in the code.

⚠️ The divider runs off **3.3 V**. 5 V on an analogue pin destroys the ADC.
⚠️ Channel must be on **ADC1** (GPIO 1–10) — ADC2 is claimed by Wi-Fi.

## Measured

| | ADC | Volts |
|---|---|---|
| Palm over sensor | ~1200 | 0.97 |
| Room light | ~3100 | 2.50 |
| Noise at rest | ±12 (<1%) | |

Thresholds **1450 / 1950** sit at roughly ⅓ and ⅔ of the span. The 500-count
hysteresis band is **about 20× wider than the ADC noise**.

## Architecture

```
ldr (where the number comes from) → twilight (what it means) → relay (what to do)
```

| Module | Knows about | Doesn't know about |
|---|---|---|
| `lib/ldr` | ADC1; channel arrives as a parameter | thresholds, the relay |
| `lib/twilight` | **numbers only** | GPIO, ADC, FreeRTOS |
| `lib/relay` | GPIO, the inverting stage | light levels |
| `src/main.c` | how to wire them together | how any of them work |

No module reads `app_config.h` — pin and channel are passed into `*_init()`.
So `lib/ldr` drops into another project unchanged, and `lib/twilight` compiles
with plain `cc` on a laptop.

## Build

```bash
pio run                       # firmware
pio run -t upload -t monitor  # flash and watch the log
pio test -e native            # logic tests, no board needed
```

## Tests

Four tests, all about hysteresis — the one property the design exists for.
Verified by mutation: collapsing the two thresholds into one makes
`test_stays_on_through_dead_zone` and `test_noise_at_threshold_does_not_chatter`
fail, the latter reporting **201 switches instead of 1**.

## Hysteresis on real hardware

From a serial capture. A hand slowly covers the sensor and wavers at the edge:

| t (ms) | ADC | Zone | Lamp | Switches |
|---|---|---|---|---|
| 25146 | 1607 | DEAD ZONE | ON | 7 |
| 25946 | 1419 | DARK | ON | 7 |
| 26146 | 1455 | DEAD ZONE | ON | 7 |
| 26946 | 1339 | DARK | ON | 7 |
| 27546 | 1481 | DEAD ZONE | ON | 7 |
| 27746 | 1327 | DARK | ON | 7 |
| 28146 | 1528 | DEAD ZONE | ON | 7 |

The signal crossed the 1450 mark **at least six times over 4.2 seconds** and the
counter never moved. With a single threshold, every one of those crossings would
have been a relay click.

## Editor setup

`.clangd` and `compile_commands.json` are not in the repository — they contain
absolute paths to this machine's toolchain.

```bash
pio run -t compiledb
```

Then point `.clangd`'s `CompilationDatabase` at `.pio/build/esp32-s3-devkitc-1`,
drop the flags clang doesn't know (`-mlongcalls`, `-specs=*`, `-fno-malloc-dce`,
`-fzero-init-padding-bits=*`), and add `-I` for the toolchain's picolibc headers.
