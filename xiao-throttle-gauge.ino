/************************************************************
 * Xiao Throttle Gauge
 * By John M. Wargo
 * https://johnwargo.com
 *
 ************************************************************/

#include <FastLED.h>
#define NUM_LEDS 7
#define DATA_PIN 1

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

void loop() {
  // put your main code here, to run repeatedly:
}
