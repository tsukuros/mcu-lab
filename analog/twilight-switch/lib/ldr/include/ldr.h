#ifndef LDR_H
#define LDR_H

#include "esp_err.h"
#include "hal/adc_types.h"

esp_err_t ldr_init(adc_channel_t channel);

/** Raw 0..4095, or -1 on failure. */
int ldr_read_raw(void);

/** Approximate: attenuator full scale is ~3.1 V, so this over-reads by ~6% at the top. */
float ldr_raw_to_volts(int raw);

#endif
