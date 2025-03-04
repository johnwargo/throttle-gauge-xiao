/************************************************************
 * Throttle Gauge (XIAO)
 * By John M. Wargo
 * https://johnwargo.com
 *
 ************************************************************/

#include <FastLED.h>

#define NUM_LEDS 7
#define THROTTLE_PIN A0
#define DATA_PIN 1

CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\nThrottle Gauge");
  // initialize the FastLED library
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  // check to make sure we have an appropriate amount of NeoPixels to make a gauge
  if (NUM_LEDS < 5) {
    Serial.println("Invalid Configuration");
    Serial.println("Number of NUM_LEDS must be greater than 5.");
    while (true) delay(100);  //loops forever
  }
}

void loop() {
  // read the voltage from the throttle pin, returns values from 0 to 1023
  int throttleValue = analogRead(THROTTLE_PIN);
  // convert the voltage to a number of NeoPixels
  int illuminatedPixels = int( do some math here);
  // illuminate the appropriate number of NeoPixels Green

  // turn the rest of the NeoPixels off

  delay(10);
}
