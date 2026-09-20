# blink-modes

A non-blocking LED controller for the ESP32-S3, written in the restricted C++
subset used on microcontrollers: no heap, no STL containers, no `delay()`.
A button on an interrupt cycles the LED through three modes.

## Hardware

| Signal | GPIO | Wiring |
|---|---|---|
| LED | 15 | GPIO → 220 Ω → LED anode; cathode → GND |
| Button | 0 | onboard BOOT button, `INPUT_PULLUP`, FALLING |

220 Ω against a green LED draws under 6 mA — well inside the 20 mA a pin should
be asked for.

The mode button is the board's onboard BOOT button rather than one on the
breadboard. An external button on GPIO 4 showed a **floating input**: with the
pin idle and untouched, an interrupt counter recorded ~14 unwanted falling edges
per second, because the chip's internal pull-up is only ~45 kΩ and the far end of
the wire never reached a defined level. No debounce window can fix that — a
debounce cannot tell contact bounce from an undefined idle level. The onboard
button is hardware-filtered and registers exactly one edge per press.

GPIO 0 is a strapping pin: held LOW during reset it selects download mode.
Pressing it while the firmware runs is fine.

## Modes

`Blink → AlwaysOn → AlwaysOff → Blink`, advanced by each accepted button press.

## Layout

| Path | Role |
|---|---|
| `lib/led/` | `Led` — driver. Owns a pin and a level, knows nothing about time. |
| `lib/blinker/` | `Blinker` — policy. Owns the mode and the timing. |
| `src/app_config.h` | `Config` — every tunable, as `static constexpr`. |
| `src/main.cpp` | Wiring, the superloop, and the ISR boundary. |

The split is the load-bearing decision: **`Led` exposes only `init()` and
`set(LedState)`**, so timing cannot live inside it. Same separation as
`analog/twilight-switch`, where `relay.c` is dumb and `twilight.c` holds policy.

`Blinker::update()` takes `nowMs` as an argument instead of calling `millis()`
itself, so it carries no Arduino dependency — the property that made
`lib/twilight` unit-testable on the host.

Two enums, deliberately: `LedState` is what the pin is doing now, `Mode` is what
the LED is for. Collapsing them makes mode cycling unimplementable. Both are
`enum class` — scoped, and not implicitly convertible to `int` or `bool`.

## Interrupt boundary

The ISR sets one `volatile bool` and returns. Debounce and mode switching happen
in `loop()`, never in the handler.

An edge alone is not enough to act on: releasing a contact bounces too, and some
of those edges fall. So a first edge only starts a 50 ms timer, and when it
expires the pin is **read again** — still LOW means a real press, HIGH means a
release that has settled. Acting on the edge alone made one press advance two
modes whenever the button was held longer than the debounce window.

Trade-off: a tap shorter than 50 ms is released before the re-read and is
ignored. Human presses run 80–200 ms.

Reading the flag and clearing it are also two separate operations, so a press
arriving in that window is dropped — acceptable for a finger, not for a pulse
train that must not miss an edge.

## Loop timing

`loop()` accumulates `micros()` per iteration and reports the mean every 1000
passes.

> Measured: **1 µs** per iteration, averaged over 1 000 000 passes.

`micros()` has 1 µs resolution and wraps every ~71 minutes — far sooner than
`millis()`. Both elapsed checks subtract before comparing, which is what makes
them survive the wrap.

The report sits outside the timed window, so the average is the cost of the work
rather than of the printing.

`reportEveryIterations` is 1 000 000, not the 1000 the exercise suggests. At 1 µs
per pass, 1000 iterations elapse in a millisecond — roughly a thousand lines per
second, which saturates the link and makes the log unreadable. A million passes
gives about one line per second. The reporting interval has to be chosen against
the measured loop speed; it is not a constant that can be fixed in advance.

## Build

```
pio run                 # build
pio run -t upload       # flash
pio device monitor      # serial
```

`ARDUINO_USB_CDC_ON_BOOT=1` is set in `platformio.ini`. Without it `Serial` goes
to UART0 on the *other* USB-C port and the monitor stays silent.

`build_src_flags` re-arms `-Wunused-parameter`, which the platform disables
globally; `lib/*/library.json` does the same for the libraries.

## Editor

```
pio run -t compiledb
```

Writes `compile_commands.json` to the project root; `.clangd` points at it. Both
are gitignored — they carry absolute paths for one machine.
