#include "gpio.h"

#include "driver/gpio.h"

/* gpio_config_t is uninitialised on the stack: set all five fields, or a
   leftover intr_type enables an interrupt nobody asked for. */
namespace gpio {

void configureOutput(uint8_t pin)
{
  gpio_config_t cfg;
  cfg.pin_bit_mask = 1ULL << pin;
  cfg.mode         = GPIO_MODE_OUTPUT;
  cfg.pull_up_en   = GPIO_PULLUP_DISABLE;
  cfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
  cfg.intr_type    = GPIO_INTR_DISABLE;
  gpio_config(&cfg);
}

void configureInput(uint8_t pin, Pull pull)
{
  gpio_config_t cfg;
  cfg.pin_bit_mask = 1ULL << pin;
  cfg.mode         = GPIO_MODE_INPUT;
  cfg.pull_up_en =
    (pull == Pull::Up) ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE;
  cfg.pull_down_en =
    (pull == Pull::Down) ? GPIO_PULLDOWN_ENABLE : GPIO_PULLDOWN_DISABLE;
  cfg.intr_type = GPIO_INTR_DISABLE;
  gpio_config(&cfg);
}

} // namespace gpio
