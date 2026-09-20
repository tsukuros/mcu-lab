#pragma once

#include <cstdint>

struct Config {
  static constexpr uint8_t  ledPin                = 15;
  static constexpr uint32_t blinkIntervalMs       = 500;
  static constexpr uint8_t  buttonPin             = 0;
  static constexpr uint32_t debounceMs            = 50;
  static constexpr uint32_t reportEveryIterations = 1000000;
  static constexpr uint32_t serialBaud            = 115200;
};
