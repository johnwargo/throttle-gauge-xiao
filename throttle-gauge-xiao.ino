/************************************************************
 * Throttle Gauge (XIAO ESP32C6)
 * By John M. Wargo
 * https://johnwargo.com
 * 
 * Created March 3, 2025
 ************************************************************/

#include <FastLED.h>

#define DEBUG

#define NUM_BRAKE_LEDS 5
#define NUM_THROTTLE_LEDS 15

#define BRAKE_LED_PIN 3
#define THROTTLE_LED_PIN 2

#define INPUT_BRAKE A0
#define INPUT_THROTTLE A1
#define THROTTLE_MIN 600

// FastLED LED Arrays
CRGB tLeds[NUM_THROTTLE_LEDS];
CRGB bLeds[NUM_BRAKE_LEDS];

// The Arduino reads between 0 and 1023 while the ESP32 reads between 0 and 4095
// Adjusts maximum voltage levels read by the device will adjust this based on
// the total voltage output from throttle to ensure we can max out the gauge
// const int inputDivisor = 1023;  // for Arduino devices
// const int inputDivisor = 4095;  // for ESP32 devices
const int inputDivisor = 4095;  // for ESP32 devices

int prevThrottleState = -1;
bool prevBrakeState = false;
bool prevMaxThrottle = false;

void setup() {

  Serial.begin(115200);
  delay(1000);
  Serial.println("\nThrottle Gauge");
  Serial.println("By John M. Wargo\n");

  // make sure we have an appropriate amount of NeoPixels to make a gauge
  if (NUM_THROTTLE_LEDS < 10) {
    Serial.println("\nInvalid Configuration\nNumber of THROTTLE_LEDS must be greater than 9");
    while (true) delay(100);  //loops forever
  }
  if (NUM_BRAKE_LEDS < 1) {
    Serial.println("\nInvalid Configuration\nNumber of BRAKE_LEDS must be greater than 0");
    while (true) delay(100);  //loops forever
  }

  FastLED.setBrightness(255);
  FastLED.addLeds<NEOPIXEL, THROTTLE_LED_PIN>(tLeds, NUM_THROTTLE_LEDS);
  FastLED.addLeds<NEOPIXEL, BRAKE_LED_PIN>(bLeds, NUM_BRAKE_LEDS);
  testLEDs();

  pinMode(INPUT_THROTTLE, INPUT);
  pinMode(INPUT_BRAKE, INPUT);
}

void loop() {
  updateThrottle();
  updateBrake();
  FastLED.show();
  delay(25);
}

void updateThrottle() {

  // Conditions
  // throttle value is the same as previous - do nothing
  // throttle value was greater than 80% and is greater than 80% now - do nothing
  // throttle value is different than last time - update LED state

  int throttleValue;
  int numIlluminatedPixels;
  float pixelRatio;
  bool maxThrottle;
  bool stateChange;

  // read the voltage from the throttle pin
  throttleValue = analogRead(INPUT_THROTTLE);
#ifdef DEBUG
  Serial.printf("Throttle Value: %d\n", throttleValue);
#endif

  // calculate the pixel ratio
  pixelRatio = (float)throttleValue / (float)inputDivisor;
  maxThrottle = pixelRatio >= 0.80;  // 80% or more means max throttle
  // Did the state change in any way?
  stateChange = (throttleValue != prevThrottleState) || (maxThrottle != prevMaxThrottle);
  // reset our `prev` values for next check
  prevThrottleState = throttleValue;
  prevMaxThrottle = maxThrottle;

  if (stateChange) {
    if (maxThrottle) {
      // Set all throttle LEDs to WHITE
      for (int j = 0; j < NUM_THROTTLE_LEDS; j++) tLeds[j] = CRGB::White;
    } else {
      if (throttleValue > THROTTLE_MIN) {
        numIlluminatedPixels = pixelRatio * NUM_THROTTLE_LEDS;
      } else {
        numIlluminatedPixels = 0;
      }
#ifdef DEBUG
      Serial.printf("Pixels: %d\n", numIlluminatedPixels);
#endif
      if (numIlluminatedPixels > 0) {
        // light the green ones based on the throttle value
        for (int j = 0; j < NUM_THROTTLE_LEDS; j++) tLeds[j] = CRGB::Green;        
      }
    }
  }
}

void updateBrake() {
}

void testLEDs() {
#define testDelay 18
  Serial.println("Testing LEDs");

  FastLED.clear();
  FastLED.show();
  for (int j = 0; j < NUM_THROTTLE_LEDS; j++) {
    tLeds[j] = CRGB::Green;
    FastLED.show();
    delay(testDelay);
  }
  for (int j = NUM_THROTTLE_LEDS - 1; j > -1; j--) {
    tLeds[j] = CRGB::Black;
    FastLED.show();
    delay(testDelay);
  }
  FastLED.clear();
  FastLED.show();
  fill_solid(bLeds, NUM_BRAKE_LEDS, CRGB::Red);
  FastLED.show();
  delay(1000);
  FastLED.clear();
  FastLED.show();
}
