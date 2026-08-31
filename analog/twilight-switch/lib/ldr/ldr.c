#include "ldr.h"
#include "esp_adc/adc_oneshot.h"

static adc_oneshot_unit_handle_t s_adc1;
static adc_channel_t             s_channel;

esp_err_t ldr_init(adc_channel_t channel)
{
  s_channel = channel;

  adc_oneshot_unit_init_cfg_t unit_cfg = {
    .unit_id = ADC_UNIT_1,
  };

  esp_err_t err = adc_oneshot_new_unit(&unit_cfg, &s_adc1);

  if (err != ESP_OK) {
    return err;
  }

  adc_oneshot_chan_cfg_t chan_cfg = {
    .atten    = ADC_ATTEN_DB_12,
    .bitwidth = ADC_BITWIDTH_DEFAULT,
  };

  return adc_oneshot_config_channel(s_adc1, s_channel, &chan_cfg);
}

int ldr_read_raw(void)
{
  int raw = -1;
  adc_oneshot_read(s_adc1, s_channel, &raw);
  return raw;
}

float ldr_raw_to_volts(int raw)
{
  return (raw / 4095.0f) * 3.3f;
}
