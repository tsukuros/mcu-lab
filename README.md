# mcu-lab

Microcontroller firmware experiments. Boards: **ESP32-S3 N16R8**,
**STM32F411 Black Pill**. Built with PlatformIO, mostly on ESP-IDF.

## Layout

Directories are grouped by topic, not chronology:

```
analog/       ADC, sensors, voltage dividers
shared/       code used by more than one project
```

`timing/`, `control/`, `interfaces/` and `rtos/` appear as projects arrive.

Each directory under a topic is a **self-contained PlatformIO project**.
Enter it, run `pio run`, done. No shared state between projects.

### Projects

| Project | What it does |
|---|---|
| [`analog/twilight-switch`](analog/twilight-switch/) | photoresistor → ADC → hysteresis → transistor switch → relay |

## Building

```bash
cd analog/twilight-switch && pio run
```

Logic that doesn't touch hardware is unit-tested on the host:

```bash
pio test -e native
```

> After editing any `CMakeLists.txt`, run `rm -rf .pio build`. CMake caches the
> component graph and keeps using the stale one — the error then looks exactly
> like a bug you have already fixed.

## CI

[`.github/workflows/build.yml`](.github/workflows/build.yml) runs on pushes to
`main` and on pull requests targeting it. Host tests go first (seconds, no
hardware), then firmware builds — broken logic never reaches a build.

The matrix is discovered from the tree: any directory containing a
`platformio.ini` is picked up, so adding a project needs no CI change. A project
without an `[env:native]` section simply skips the test stage.

Work lands through pull requests, so every module arrives with a green build
behind it.

## Snapshots

Point-in-time states are **tags** (`twilight-switch-v1`), not branches.
Branches model parallel work that later merges; an archive of finished work
is not that.
