#pragma once

#include <stdint.h>

class Relay {
public:
  explicit Relay(uint8_t coilPin);

  void init();
  void set(bool on);
  bool isOn() const;

private:
  uint8_t coilPin_;
  bool    on_;
};
