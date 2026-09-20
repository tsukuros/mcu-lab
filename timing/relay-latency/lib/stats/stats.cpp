#include "stats.h"

#include <stdint.h>

Stats::Stats()
{
  reset();
}

void Stats::reset()
{
  sum_   = 0;
  min_   = UINT32_MAX;
  max_   = 0;
  count_ = 0;
}

void Stats::add(uint32_t sample)
{
  sum_ += sample;
  min_ = sample < min_ ? sample : min_;
  max_ = sample > max_ ? sample : max_;
  count_++;
}

uint16_t Stats::count() const
{
  return count_;
}

uint32_t Stats::min() const
{
  return count_ == 0 ? 0 : min_;
}

uint32_t Stats::max() const
{
  return max_;
}

uint32_t Stats::mean() const
{
  return count_ == 0 ? 0 : (sum_ + count_ / 2) / count_;
}
