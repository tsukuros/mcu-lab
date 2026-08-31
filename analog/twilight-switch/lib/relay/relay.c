#include "relay.h"
#include "driver/gpio.h"

static int s_gpio = -1;

esp_err_t relay_init(int gpio)
{
  s_gpio = gpio;

  gpio_config_t gpio_cfg = {
    .pin_bit_mask = 1ULL << gpio,
    .mode         = GPIO_MODE_OUTPUT,
  };

  esp_err_t err = gpio_config(&gpio_cfg);

  if (err != ESP_OK) {
    return err;
  }

  relay_set(false);

  return ESP_OK;
}

void relay_set(bool on)
{
  gpio_set_level(s_gpio, on ? 1 : 0);
}
