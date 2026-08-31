#ifndef RELAY_H
#define RELAY_H

#include <stdbool.h>
#include "esp_err.h"

/* BC547B inverts, relay module is active-LOW: the two cancel out,
   so HIGH on the pin means the relay is ON. */

esp_err_t relay_init(int gpio);

void relay_set(bool on);

#endif
