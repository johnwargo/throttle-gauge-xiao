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

#define BRAKE_LED_PIN 13
#define THROTTLE_LED_PIN 12

#define INPUT_BRAKE A0
#define INPUT_THROTTLE A1
#define THROTTLE_MIN 600

#define testDelay 15

// FastLED LED Arrays
CRGB tLeds[NUM_THROTTLE_LEDS];
CRGB bLeds[NUM_BRAKE_LEDS];

// The Arduino reads between 0 and 1023 while the ESP32 reads between 0 and 4095
// Adjusts maximum voltage levels read by the device will adjust this based on
// the total voltage output from throttle to ensure we can max out the gauge
// const int inputDivisor = 1023;  // for Arduino devices
// const int inputDivisor = 4095;  // for ESP32 devices
const int inputDivisor = 4095;  // for ESP32 devices
const int brakeThreshold = inputDivisor / 2;

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

  pinMode(INPUT_THROTTLE, INPUT_PULLDOWN);
  pinMode(INPUT_BRAKE, INPUT_PULLDOWN);

  // test the LEDs, show that they work
  testLEDs();
}

void loop() {
  updateThrottle();
  updateBrake();
  delay(50);
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
  bool throttleStateChange;

  // read the voltage from the throttle pin
  throttleValue = analogRead(INPUT_THROTTLE);
#ifdef DEBUG
  Serial.printf("Throttle Value: %d\n", throttleValue);
#endif

  // calculate the pixel ratio
  pixelRatio = (float)throttleValue / (float)inputDivisor;
  maxThrottle = pixelRatio >= 0.80;  // 80% or more means max throttle
  // Did the state change in any way?
  throttleStateChange = (throttleValue != prevThrottleState) || (maxThrottle != prevMaxThrottle);
  // reset our `prev` values for next check
  prevThrottleState = throttleValue;
  prevMaxThrottle = maxThrottle;

  if (throttleStateChange) {
    if (maxThrottle) {
      // Set all throttle LEDs to WHITE
      for (int i = 0; i < NUM_THROTTLE_LEDS; i++) tLeds[i] = CRGB::White;
    } else {
      // Set only a portion of the throttle LEDs to Green
      if (throttleValue > THROTTLE_MIN) {
        numIlluminatedPixels = pixelRatio * NUM_THROTTLE_LEDS;
      } else {
        numIlluminatedPixels = 0;
      }
#ifdef DEBUG
      Serial.printf("Pixels: %d\n", numIlluminatedPixels);
#endif
      if (numIlluminatedPixels > 0) {
        // light LEDs green based on the current throttle value
        for (int i = 0; i < numIlluminatedPixels; i++) tLeds[i] = CRGB::Green;
      }
      // then set the rest to black
      for (int i = numIlluminatedPixels; i < NUM_THROTTLE_LEDS; i++) tLeds[i] = CRGB::Black;
    }
    FastLED.show();
  }
}

void updateBrake() {
  CRGB ledColor;
  bool brakeState;

  brakeState = (analogRead(INPUT_BRAKE) > brakeThreshold);
  // do we have a change in brake state?
  if (brakeState != prevBrakeState) {
    prevBrakeState = brakeState;
    // then update the LEDs
    ledColor = (brakeState) ? CRGB::Red : CRGB::Black;
    for (int i = 0; i < NUM_BRAKE_LEDS; i++) bLeds[i] = ledColor;
    FastLED.show();
  }
}

void testLEDs() {
  Serial.println("Testing LEDs");

  FastLED.clear();
  FastLED.show();
  for (int i = 0; i < NUM_THROTTLE_LEDS; i++) {
    Serial.println(i);
    tLeds[i] = CRGB::Green;
    FastLED.show();
    delay(testDelay);
  }
  for (int i = NUM_THROTTLE_LEDS - 1; i > -1; i--) {
    Serial.println(i);
    tLeds[i] = CRGB::Black;
    FastLED.show();
    delay(testDelay);
  }

  for (int i = 0; i < 3; i++) {
    fill_solid(bLeds, NUM_BRAKE_LEDS, CRGB::Red);
    FastLED.show();
    delay(250);
    FastLED.clear();
    FastLED.show();
    delay(100);
  }
}
