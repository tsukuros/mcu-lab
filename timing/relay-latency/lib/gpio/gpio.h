#pragma once

#include <stdint.h>

#include "soc/gpio_struct.h"

namespace gpio {

enum class Pull : uint8_t { None, Up, Down };

void configureOutput(uint8_t pin);
void configureInput(uint8_t pin, Pull pull);

inline void set(uint8_t pin)
{
  GPIO.out_w1ts = 1u << pin;
}

inline void clear(uint8_t pin)
{
  GPIO.out_w1tc = 1u << pin;
}

inline void write(uint8_t pin, bool level)
{
  if (level) {
    set(pin);
  } else {
    clear(pin);
  }
}

inline bool read(uint8_t pin)
{
  return (GPIO.in >> pin) & 1u;
}

} // namespace gpio
