/************************************************************
 * Throttle Gauge (XIAO)
 * By John M. Wargo
 * https://johnwargo.com
 * 
 * Created March 3, 2025
 ************************************************************/

#include <FastLED.h>

#define DEBUG

#define LEDS_THROTTLE 15
#define DATA_PIN_THROTTLE 0

#define LEDS_BRAKE 5
#define DATA_PIN_BRAKE 1

#define THROTTLE_PIN A3
#define THROTTLE_MIN 600

CRGB tLeds[LEDS_THROTTLE];
CRGB bLeds[LEDS_BRAKE];

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

  // make sure we have an appropriate amount of NeoPixels to make a gauge
  if (LEDS_THROTTLE < 5) {
    Serial.println("\nInvalid Configuration\nNumber of LEDS_THROTTLE must be greater than 5");
    while (true) delay(100);  //loops forever
  }

  FastLED.setBrightness(255);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_THROTTLE>(tLeds, LEDS_THROTTLE);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_BRAKE>(bLeds, LEDS_BRAKE);
  // testLEDs();

  pinMode(A0, INPUT);
}

void loop() {

  testLEDs();
  delay(1000);

  // int throttleValue;
  // int numIlluminatedPixels;

  //   // read the voltage from the throttle pin, returns values from 0 to 1023
  //   throttleValue = analogRead(THROTTLE_PIN);
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
  for (int j = 0; j < LEDS_THROTTLE; j++) {
    tLeds[j] = CRGB::Green;
    FastLED.show();
    delay(throttleDelay);
  }
  for (int j = LEDS_THROTTLE - 1; j > -1; j--) {
    tLeds[j] = CRGB::Black;
    FastLED.show();
    delay(throttleDelay);
  }  
  FastLED.clear();
  FastLED.show();
  fill_solid(bLeds, LEDS_BRAKE, CRGB::Red);
  FastLED.show();
  delay(1000);
  FastLED.clear();
  FastLED.show();
}
