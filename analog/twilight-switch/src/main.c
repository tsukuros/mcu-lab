#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "app_config.h"
#include "ldr.h"
#include "twilight.h"
#include "relay.h"

static const char *TAG = "twilight";

void app_main(void)
{
  ESP_ERROR_CHECK(ldr_init(CFG_LDR_ADC_CHANNEL));
  ESP_ERROR_CHECK(relay_init(CFG_RELAY_GPIO));

  twilight_t state;
  twilight_init(&state, CFG_THRESHOLD_DARK, CFG_THRESHOLD_LIGHT);

  while (1) {
    const int  raw  = ldr_read_raw();
    const bool lamp = twilight_update(&state, raw);

    relay_set(lamp);

    ESP_LOGI(TAG, "adc=%4d (%.2f V)  %-10s  lamp=%-3s  switches=%lu",
             raw,
             ldr_raw_to_volts(raw),
             twilight_zone_name(twilight_zone(&state, raw)),
             lamp ? "ON" : "OFF",
             state.switches);

    vTaskDelay(pdMS_TO_TICKS(CFG_SAMPLE_PERIOD_MS));
  }
}
