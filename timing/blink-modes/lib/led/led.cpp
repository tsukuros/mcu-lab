#include "led.h"

#include <Arduino.h>

Led::Led(uint8_t pin) : pin_(pin), state_(LedState::Off) {}

void Led::init()
{
  pinMode(pin_, OUTPUT);
  set(state_);
}

void Led::set(LedState state)
{
  digitalWrite(pin_, state == LedState::On ? HIGH : LOW);
  state_ = state;
}
