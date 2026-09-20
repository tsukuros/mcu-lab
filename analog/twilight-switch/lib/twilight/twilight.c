#include "twilight.h"

void twilight_init(twilight_t *t, int threshold_dark, int threshold_light)
{
  t->threshold_dark  = threshold_dark;
  t->threshold_light = threshold_light;
  t->lamp_on         = false;
  t->switches        = 0;
}

bool twilight_update(twilight_t *t, int raw)
{
  const twilight_zone_t zone = twilight_zone(t, raw);

  if (!t->lamp_on && zone == TWILIGHT_DARK) {
    t->lamp_on = true;
    t->switches++;
  } else if (t->lamp_on && zone == TWILIGHT_LIGHT) {
    t->lamp_on = false;
    t->switches++;
  }

  return t->lamp_on;
}

twilight_zone_t twilight_zone(const twilight_t *t, int raw)
{
  if (raw < t->threshold_dark) {
    return TWILIGHT_DARK;
  } else if (raw > t->threshold_light) {
    return TWILIGHT_LIGHT;
  }

  return TWILIGHT_DEAD;
}

const char *twilight_zone_name(twilight_zone_t zone)
{
  switch (zone) {
    case TWILIGHT_DARK: return "DARK";
    case TWILIGHT_LIGHT: return "LIGHT";
    case TWILIGHT_DEAD: return "DEAD ZONE";
  }
  /* C enums are not exhaustive, so the compiler demands this line. */
  return "?";
}
