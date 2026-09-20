#include <Arduino.h>

#include "driver/gpio.h"
#include "esp_timer.h"

#include "app_config.h"
#include "gpio.h"
#include "relay.h"
#include "stats.h"

static Relay relay{Config::coilPin};
static Stats pullIn;  /* coil energised -> contact closes */
static Stats dropOut; /* coil released  -> contact opens  */

/* volatile or the compiler hoists the spin-loop read and never sees the ISR.
   64-bit edgeUs needs no lock: the ISR shares this core and writes it before
   clearing armed. */
static volatile int64_t edgeUs = 0;
static volatile bool    armed  = false;

/* The first edge is the measurement; everything after it is bounce.
   Disarming here is what discards it. */
static void IRAM_ATTR onContactChange(void *arg)
{
  (void)arg;
  if (armed) {
    edgeUs = esp_timer_get_time();
    armed  = false;
  }
}

/* Commands the coil and returns microseconds until the contact reacted,
   or 0 if it never did. */
static uint32_t measureEdgeUs(bool coilOn)
{
  edgeUs = 0;
  armed  = true;

  /* Nothing between this timestamp and the command: it all becomes error. */
  const int64_t startUs = esp_timer_get_time();
  relay.set(coilOn);

  while (armed) {
    if (esp_timer_get_time() - startUs >= Config::timeoutUs) {
      return 0;
    }
  }
  return (uint32_t)(edgeUs - startUs);
}

static void report(const char *label, const Stats &s)
{
  Serial.printf("%-9s n=%2u   min=%6u   mean=%6u   max=%6u  (us)\n",
                label,
                (uint32_t)s.count(),
                s.min(),
                s.mean(),
                s.max());
}

void setup()
{
  Serial.begin(Config::serialBaud);
  delay(500); /* USB CDC enumerates after boot; earlier prints are lost. */

  relay.init();
  gpio::configureInput(Config::contactPin, gpio::Pull::Up);

  gpio_set_intr_type((gpio_num_t)Config::contactPin, GPIO_INTR_ANYEDGE);
  gpio_install_isr_service(0);
  gpio_isr_handler_add(
    (gpio_num_t)Config::contactPin, onContactChange, nullptr);
}

void loop()
{
  /* loop() runs again the moment it returns; "finished" has to be kept. */
  static bool done = false;
  if (done) {
    delay(1000);
    return;
  }

  /* LOW here means NC was wired instead of NO, or COM is not grounded. */
  Serial.printf("\ncontact at rest: %s\n\n",
                gpio::read(Config::contactPin) ? "HIGH (open, expected)"
                                               : "LOW (check wiring)");

  for (uint16_t i = 0; i < Config::sampleCount; i++) {
    const uint32_t on = measureEdgeUs(true);
    delay(Config::settleMs);
    const uint32_t off = measureEdgeUs(false);
    delay(Config::settleMs);

    Serial.printf("%2u   pull-in %6u us   drop-out %6u us%s\n",
                  (uint32_t)(i + 1),
                  on,
                  off,
                  (on && off) ? "" : "   <- timeout");

    if (on)
      pullIn.add(on);
    if (off)
      dropOut.add(off);
  }

  Serial.println();
  report("pull-in", pullIn);
  report("drop-out", dropOut);

  relay.set(false);
  done = true;
}
