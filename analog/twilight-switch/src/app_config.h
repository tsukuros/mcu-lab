#ifndef APP_CONFIG_H
#define APP_CONFIG_H

/* ADC1 (GPIO 1-10) works even with Wi-Fi up. ADC2 (GPIO 11-20) does not. */
#define CFG_LDR_ADC_CHANNEL ADC_CHANNEL_3
#define CFG_RELAY_GPIO      15

/* Measured on this LDR: covered ~1200, room light ~3100, noise +/-12. */
#define CFG_THRESHOLD_DARK  1450
#define CFG_THRESHOLD_LIGHT 1950

#define CFG_SAMPLE_PERIOD_MS 200

#endif
