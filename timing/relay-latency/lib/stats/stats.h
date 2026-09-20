#pragma once

#include <stdint.h>

class Stats {
public:
  Stats();

  void add(uint32_t sample);
  void reset();

  uint16_t count() const;
  uint32_t min() const;
  uint32_t max() const;

  /* Rounded to nearest, not truncated. Returns 0 when count() == 0. */
  uint32_t mean() const;

private:
  uint32_t sum_;
  uint32_t min_;
  uint32_t max_;
  uint16_t count_;
};
