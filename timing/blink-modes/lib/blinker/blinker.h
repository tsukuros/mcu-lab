#pragma once

#include <stdint.h>

#include "led.h"

enum class Mode { Blink, AlwaysOn, AlwaysOff };

class Blinker {
public:
  explicit Blinker(uint32_t intervalMs);

  void     nextMode();
  Mode     mode() const;
  LedState update(uint32_t nowMs);

private:
  uint32_t intervalMs_;
  uint32_t lastToggleMs_;
  Mode     mode_;
  LedState state_;
};

const char *modeName(Mode mode);
