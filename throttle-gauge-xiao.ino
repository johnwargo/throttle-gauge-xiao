/************************************************************
 * Throttle Gauge (XIAO)
 * By John M. Wargo
 * https://johnwargo.com
 * 
 * Created March 3, 2025
 ************************************************************/

#include <FastLED.h>

#define NUM_LEDS 7
#define THROTTLE_PIN A0
#define DATA_PIN 1

// The Arduino reads between 0 and 1023
// The ESP32 reads between 0 and 4095
// Adjusts maximum voltage levels read by the device
// will adjust this based on total voltage output from throttle
// to ensure we can max out the gauge
#define DIVISOR 4095

CRGB leds[NUM_LEDS];

int previousThrottleValue = -1;

void setup() {

  Serial.begin(115200);
  delay(1000);
  Serial.println("\nThrottle Gauge");
  Serial.println("By John M. Wargo\n");

  // make sure we have an appropriate amount of NeoPixels to make a gauge
  if (NUM_LEDS < 5) {
    Serial.println("\nInvalid Configuration\nNumber of NUM_LEDS must be greater than 5");
    while (true) delay(100);  //loops forever
  }

  pinMode(A0, INPUT);

  // initialize the FastLED library
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  testLEDs();
}

void loop() {
  // read the voltage from the throttle pin, returns values from 0 to 1023
  int throttleValue = analogRead(THROTTLE_PIN);
  if (throttleValue != previousThrottleValue) {
    // reset the previous throttle value
    previousThrottleValue = throttleValue;

    Serial.printf("Throttle Value: %d\n", throttleValue);
    Serial.printf("Divisor: %d\n", DIVISOR);
    Serial.printf("Ratio: %f\n", throttleValue / DIVISOR);

    // update the gauge; convert the voltage to a number of NeoPixels
    float tmpVal = (throttleValue / DIVISOR) * NUM_LEDS;
    Serial.printf("Value: %f\n", tmpVal);
    int numIlluminatedPixels = int(tmpVal);
    Serial.printf("Pixels: %d\n", numIlluminatedPixels);
    FastLED.clear();
    if (numIlluminatedPixels > 0) {
      // light the green ones based on the throttle value
      fill_solid(leds, throttleValue, CRGB::Green);
    }
    FastLED.show();
    delay(100);  // just for testing
  } else {
    delay(25);
  }
}

void testLEDs() {

  int numColors = 3;
  uint32_t colors[] = { CRGB::Red, CRGB::Green, CRGB::Blue };

  for (int i = 0; i < numColors; i++) {
    FastLED.clear();
    FastLED.show();
    for (int j = 0; j < NUM_LEDS; j++) {
      leds[j] = colors[i];
      FastLED.show();
      delay(25);
    }
    for (int j = NUM_LEDS - 1; j > -1; j--) {
      leds[j] = CRGB::Black;
      FastLED.show();
      delay(25);
    }
    delay(500);
  }
}
