#ifndef TWILIGHT_H
#define TWILIGHT_H

#include <stdbool.h>
#include <stdint.h>

/* No hardware dependencies, which is why this runs under test on the host. */

typedef enum {
    TWILIGHT_DARK,
    TWILIGHT_DEAD,
    TWILIGHT_LIGHT
} twilight_zone_t;

typedef struct {
    int      threshold_dark;
    int      threshold_light;
    bool     lamp_on;
    uint32_t switches;
} twilight_t;

void twilight_init(twilight_t *t, int threshold_dark, int threshold_light);

/** Feed a sample, get the lamp state. Inside the dead zone nothing changes. */
bool twilight_update(twilight_t *t, int raw);

twilight_zone_t twilight_zone(const twilight_t *t, int raw);

const char *twilight_zone_name(twilight_zone_t zone);

#endif
