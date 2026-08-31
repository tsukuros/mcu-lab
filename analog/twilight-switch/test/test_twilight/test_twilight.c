#include "twilight.h"
#include <unity.h>

#define DARK 1450
#define LIGHT 1950

static twilight_t t;

void setUp(void) { twilight_init(&t, DARK, LIGHT); }
void tearDown(void) {}

static void test_turns_on_when_dark(void) {
  TEST_ASSERT_TRUE(twilight_update(&t, 1200));
  TEST_ASSERT_EQUAL_UINT32(1, t.switches);
}

static void test_turns_off_when_light(void) {
  twilight_update(&t, 1200);
  TEST_ASSERT_FALSE(twilight_update(&t, 3100));
  TEST_ASSERT_EQUAL_UINT32(2, t.switches);
}

static void test_stays_on_through_dead_zone(void) {
  twilight_update(&t, 1200);
  TEST_ASSERT_TRUE(twilight_update(&t, 1700));
  TEST_ASSERT_TRUE(twilight_update(&t, 1900));
  TEST_ASSERT_EQUAL_UINT32(1, t.switches);
}

static void test_noise_at_threshold_does_not_chatter(void) {
  twilight_update(&t, 1440);
  for (int i = 0; i < 100; i++) {
    twilight_update(&t, 1460);
    twilight_update(&t, 1440);
  }
  TEST_ASSERT_EQUAL_UINT32(1, t.switches);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_turns_on_when_dark);
  RUN_TEST(test_turns_off_when_light);
  RUN_TEST(test_stays_on_through_dead_zone);
  RUN_TEST(test_noise_at_threshold_does_not_chatter);
  return UNITY_END();
}
