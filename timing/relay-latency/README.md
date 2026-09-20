# relay-latency

Measures the mechanical delay of an electromechanical relay: how long after the
coil is commanded the contact actually moves. One GPIO drives the coil, another
reads the dry contact, and the gap between them is timed with `micros()` from an
interrupt.

## Circuit

```
              +5V ──[ relay module VCC ]
                                                      ┌─ NO ── GPIO 16 (INPUT_PULLUP)
GPIO 17 ──[ 10k ]────── B ┤ BC547B ── C ── IN (relay) ─┤
                          │ E                          └─ COM ── GND
              GND ────────┴── common ground
```

The coil side is the twilight switch's stage unchanged. The new half is the
**dry contact**: `COM` to ground, `NO` to a pin held up by the internal pullup.
Contact open reads HIGH, contact closed reads LOW — so the contact tells the
MCU about itself with no extra parts and no shared supply.

⚠️ `COM`/`NO` are galvanically isolated from the coil. That is the whole point of
a relay, and it is also why reading them costs nothing in noise.

On the SRD-05VDC-SL-C module the contact terminals are screw-clamped and labelled
in Chinese: **常开 = NO**, **公共端 = COM**, **常闭 = NC**, left to right. The
"10A 250VAC" rating on the can is a maximum, not a minimum -- switching a logic
level through it is fine.

## How the measurement works

| | Command | Contact | Edge |
|---|---|---|---|
| Pull-in | coil ON | opens → closes | **falling** |
| Drop-out | coil OFF | closes → opens | **rising** |

`attachInterrupt(..., CHANGE)` catches both. The ISR records `micros()` of the
**first** edge and disarms itself; every edge after that is contact bounce and
is discarded.

> This is the inverse of the debounce in [`../blink-modes`](../blink-modes):
> there the first edge was noise and the settled level was the answer. Here the
> first edge *is* the answer. Same interrupt machinery, opposite filter.

## Results

Ten cycles, from a serial capture.

| | n | min | mean | max | spread |
|---|---|---|---|---|---|
| Pull-in | 10 | 3346 µs | **3361 µs** | 3372 µs | 26 µs (0.8%) |
| Drop-out | 10 | 4563 µs | **4571 µs** | 4583 µs | 20 µs (0.4%) |

Two things worth stating.

**Drop-out is slower than pull-in**, by about 1.2 ms. Intuition says the
opposite -- a spring should beat an electromagnet. The flyback diode is why it
doesn't. On release it hands the collapsing field a low-resistance loop to
circulate in, so coil current decays on a slow L/R curve instead of being
forced to zero. Sustained current means a sustained field, which holds the
armature down after the command has gone.

The diode that protects the transistor is the same component that slows the
release. The usual fix, where release time matters, is a resistor or zener in
series with the diode: a higher clamp voltage collapses the current faster, at
the cost of more stress on the switching device. Not free either way.

**A mechanical part repeating to better than 1%** is the other surprise. The
armature travels the same distance against the same spring every time, and
26 µs of scatter on 3.4 ms approaches what the measurement itself can resolve.
Contact *bounce* is chaotic; contact *actuation* is not.

## Build

```bash
pio run                       # firmware
pio run -t upload -t monitor  # flash and watch the log
pio test -e native            # averaging logic, no board needed
```
