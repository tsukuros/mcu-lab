#pragma once

#include <cstdint>

struct Config {
  /* 10k suits a relay module input (microamps). A motor would need ~87 ohm
     and would still overrun the pin. */
  static constexpr uint8_t coilPin = 17;

  /* COM -> GND, NO -> this pin. Open reads HIGH, closed reads LOW. */
  static constexpr uint8_t contactPin = 16;

  static constexpr uint16_t sampleCount = 10;

  /* Generous ceiling that still fails fast when the contact is unwired. */
  static constexpr uint32_t timeoutUs = 100000;

  /* Let the armature rest, or measurement N carries N-1 with it. */
  static constexpr uint32_t settleMs = 300;

  static constexpr uint32_t serialBaud = 115200;
};
