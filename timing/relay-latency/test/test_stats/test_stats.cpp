#include <unity.h>

#include "stats.h"

void setUp(void) {}
void tearDown(void) {}

static void test_empty_reports_zero(void)
{
  Stats s;
  TEST_ASSERT_EQUAL_UINT16(0, s.count());
  TEST_ASSERT_EQUAL_UINT32(0, s.mean());
}

static void test_single_sample_is_its_own_mean_min_max(void)
{
  Stats s;
  s.add(7000);
  TEST_ASSERT_EQUAL_UINT16(1, s.count());
  TEST_ASSERT_EQUAL_UINT32(7000, s.mean());
  TEST_ASSERT_EQUAL_UINT32(7000, s.min());
  TEST_ASSERT_EQUAL_UINT32(7000, s.max());
}

static void test_tracks_min_and_max(void)
{
  Stats s;
  s.add(9000);
  s.add(4000);
  s.add(6000);
  TEST_ASSERT_EQUAL_UINT32(4000, s.min());
  TEST_ASSERT_EQUAL_UINT32(9000, s.max());
}

/* 10/3 is 3.33 -> 3, but 20/3 is 6.67 -> 7. Truncation would give 6. */
static void test_mean_rounds_to_nearest_not_down(void)
{
  Stats s;
  s.add(6);
  s.add(7);
  s.add(7);
  TEST_ASSERT_EQUAL_UINT32(7, s.mean());
}

static void test_reset_clears_everything(void)
{
  Stats s;
  s.add(1234);
  s.reset();
  TEST_ASSERT_EQUAL_UINT16(0, s.count());
  TEST_ASSERT_EQUAL_UINT32(0, s.mean());
  s.add(10);
  TEST_ASSERT_EQUAL_UINT32(10, s.min());
}

int main(int, char **)
{
  UNITY_BEGIN();
  RUN_TEST(test_empty_reports_zero);
  RUN_TEST(test_single_sample_is_its_own_mean_min_max);
  RUN_TEST(test_tracks_min_and_max);
  RUN_TEST(test_mean_rounds_to_nearest_not_down);
  RUN_TEST(test_reset_clears_everything);
  return UNITY_END();
}
