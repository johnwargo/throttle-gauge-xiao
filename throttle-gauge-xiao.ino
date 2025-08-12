/************************************************************
 * Throttle Gauge (XIAO)
 * By John M. Wargo
 * https://johnwargo.com
 * 
 * Created March 3, 2025
 ************************************************************/

#include <FastLED.h>

#define DEBUG

#define BRAKE_LEDS 5
#define THROTTLE_LEDS 15

#define BRAKE_PIN 3
#define THROTTLE_PIN 2

#define INPUT_BRAKE A0
#define INPUT_THROTTLE A1
#define THROTTLE_MIN 600

CRGB tLeds[THROTTLE_LEDS];
CRGB bLeds[BRAKE_LEDS];

// The Arduino reads between 0 and 1023 while the ESP32 reads between 0 and 4095
// Adjusts maximum voltage levels read by the device will adjust this based on
// the total voltage output from throttle to ensure we can max out the gauge
// const int divisor = 1023;  // for Arduino devices
// const int divisor = 4095;   // for ESP32 devices
const int divisor = 1650;  // for ESP32 devices
int previousThrottleValue = -1;

void setup() {

  Serial.begin(115200);
  delay(1000);
  Serial.println("\nThrottle Gauge");
  Serial.println("By John M. Wargo\n");

  // m#define BRAKE_LEDS 5ake sure we have an appropriate amount of NeoPixels to make a gauge
  if (THROTTLE_LEDS < 5) {
    Serial.println("\nInvalid Configuration\nNumber of THROTTLE_LEDS must be greater than 5");
    while (true) delay(100);  //loops forever
  }

  FastLED.setBrightness(255);
  FastLED.addLeds<NEOPIXEL, THROTTLE_PIN>(tLeds, THROTTLE_LEDS);
  FastLED.addLeds<NEOPIXEL, BRAKE_PIN>(bLeds, BRAKE_LEDS);
  // testLEDs();

  pinMode(INPUT_THROTTLE, INPUT);
  pinMode(INPUT_BRAKE, INPUT);
}

void loop() {

  testLEDs();
  delay(1000);

  // int throttleValue;
  // int numIlluminatedPixels;

  //   // read the voltage from the throttle pin, returns values from 0 to 1023
  //   throttleValue = analogRead(INPUT_THROTTLE);
  //   if (throttleValue != previousThrottleValue) {
  // #ifdef DEBUG
  //     Serial.printf("Throttle Value: %d\n", throttleValue);
  // #endif
  //     // reset the previous throttle value
  //     previousThrottleValue = throttleValue;
  //     if (throttleValue > THROTTLE_MIN) {
  //       // update the gauge; convert the voltage to a number of NeoPixels
  //       // Calculate the ratio as a float but then save it to to `numIlluminatedPixels` as an
  //       // integer result of the integer multiplication.
  //       numIlluminatedPixels = ((float)throttleValue / (float)divisor) * LEDS_THROTTLE;
  //     } else {
  //       numIlluminatedPixels = 0;
  //     }
  //     // Serial.printf("Pixels: %d\n", numIlluminatedPixels);
  //     FastLED.clear();
  //     if (numIlluminatedPixels > 0) {
  //       // light the green ones based on the throttle value
  //       fill_solid(leds, numIlluminatedPixels, CRGB::Green);
  //     }
  //     FastLED.show();
  // #ifdef DEBUG
  //     delay(500);  // just for testing
  // #endif
  //   } else {
  //     delay(25);
  //   }
}

void testLEDs() {

  int throttleDelay = 18;
  // int numColors = 3;
  // uint32_t colors[] = { CRGB::Red, CRGB::Green, CRGB::Blue };

  Serial.println("Testing LEDs");

  FastLED.clear();
  FastLED.show();
  for (int j = 0; j < THROTTLE_LEDS; j++) {
    tLeds[j] = CRGB::Green;
    FastLED.show();
    delay(throttleDelay);
  }
  for (int j = THROTTLE_LEDS - 1; j > -1; j--) {
    tLeds[j] = CRGB::Black;
    FastLED.show();
    delay(throttleDelay);
  }  
  FastLED.clear();
  FastLED.show();
  fill_solid(bLeds, BRAKE_LEDS, CRGB::Red);
  FastLED.show();
  delay(1000);
  FastLED.clear();
  FastLED.show();
}
