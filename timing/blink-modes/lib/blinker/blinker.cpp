#include "blinker.h"

Blinker::Blinker(uint32_t intervalMs)
    : intervalMs_(intervalMs), lastToggleMs_(0), mode_(Mode::Blink),
      state_(LedState::Off)
{
}

void Blinker::nextMode()
{
  switch (mode_) {
    case Mode::Blink: mode_ = Mode::AlwaysOn; break;
    case Mode::AlwaysOn: mode_ = Mode::AlwaysOff; break;
    case Mode::AlwaysOff: mode_ = Mode::Blink; break;
  }
}

Mode Blinker::mode() const
{
  return mode_;
}

LedState Blinker::update(uint32_t nowMs)
{
  switch (mode_) {
    case Mode::AlwaysOn: return state_ = LedState::On;
    case Mode::AlwaysOff: return state_ = LedState::Off;
    case Mode::Blink:
      /* Subtract first. Unsigned wraparound keeps this correct across the
         49-day millis() rollover; lastToggleMs_ + intervalMs_ does not. */
      if (nowMs - lastToggleMs_ >= intervalMs_) {
        lastToggleMs_ = nowMs;
        return state_ = state_ == LedState::Off ? LedState::On : LedState::Off;
      }
      break;
  }
  /* enum class is scoped but not exhaustive, so this line is required. */
  return state_;
}

const char *modeName(Mode mode)
{
  switch (mode) {
    case Mode::Blink: return "BLINK";
    case Mode::AlwaysOn: return "ON";
    case Mode::AlwaysOff: return "OFF";
  }
  return "?";
}
