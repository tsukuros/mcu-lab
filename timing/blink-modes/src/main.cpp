#include <Arduino.h>

#include "app_config.h"
#include "blinker.h"
#include "led.h"

static Led     led{Config::ledPin};
static Blinker blinker{Config::blinkIntervalMs};

/* Written in the ISR, read in loop(). Without volatile the compiler may hoist
   the read out of the loop and never see a press. */
static volatile bool buttonEvent = false;

/* IRAM_ATTR keeps the handler in RAM, so it still runs while flash is busy. */
void IRAM_ATTR onButtonPress()
{
  buttonEvent = true;
}

static bool takeButtonPress(uint32_t now)
{
  static uint32_t candidateMs = 0;
  static bool     pending     = false;

  if (buttonEvent) {
    buttonEvent = false;
    if (!pending) {
      pending     = true;
      candidateMs = now;
    }
  }

  if (pending && now - candidateMs >= Config::debounceMs) {
    pending = false;
    /* Re-read rather than trust the edge: a release bounces too, and by now
       it has settled HIGH. Still LOW means a real press. */
    return digitalRead(Config::buttonPin) == LOW;
  }
  return false;
}

void setup()
{
  Serial.begin(Config::serialBaud);
  led.init();
  pinMode(Config::buttonPin, INPUT_PULLUP);
  attachInterrupt(
    digitalPinToInterrupt(Config::buttonPin), onButtonPress, FALLING);
}

void loop()
{
  static uint32_t elapsedUs  = 0;
  static uint32_t iterations = 0;
  const uint32_t  now        = millis();
  const uint32_t  startUs    = micros();

  led.set(blinker.update(now));

  if (takeButtonPress(now)) {
    blinker.nextMode();
    Serial.printf("mode: %s\n", modeName(blinker.mode()));
  }

  elapsedUs += micros() - startUs;
  iterations++;

  if (iterations == Config::reportEveryIterations) {
    Serial.printf("avg loop: %u us over %u iterations\n",
                  elapsedUs / iterations,
                  iterations);

    elapsedUs  = 0;
    iterations = 0;
  }
}
