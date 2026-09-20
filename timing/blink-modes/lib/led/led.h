#pragma once

#include <stdint.h>

enum class LedState { Off, On };

class Led {
public:
  explicit Led(uint8_t pin);

  void init();
  void set(LedState state);

private:
  uint8_t  pin_;
  LedState state_;
};
