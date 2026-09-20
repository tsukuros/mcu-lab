#include "relay.h"

#include "gpio.h"

Relay::Relay(uint8_t coilPin) : coilPin_(coilPin), on_(false) {}

void Relay::init()
{
  gpio::configureOutput(coilPin_);
  set(on_);
}

void Relay::set(bool on)
{
  on_ = on;
  gpio::write(coilPin_, on_);
}

bool Relay::isOn() const
{
  return on_;
}
