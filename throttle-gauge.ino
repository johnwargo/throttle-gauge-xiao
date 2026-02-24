/************************************************************
 * Throttle Gauge
 * By John M. Wargo
 * https://johnwargo.com
 * 
 * Created March 3, 2025
 ************************************************************/

#include <FastLED.h>

#define DEBUG

#define testDelay 15  //ms

#define NUM_BRAKE_LEDS 5
#define NUM_THROTTLE_LEDS 15

#define BRAKE_LED_PIN 12
#define THROTTLE_LED_PIN 13

#define INPUT_BRAKE A1
#define INPUT_THROTTLE A0

// February 8, rethinking max voltage readings
// Max voltage readings: 3276 -> 3931
// 0.4V = 328 (measured on the car)
#define THROTTLE_MIN 450
#define THROTTLE_MAX 2000  //was 3500

// FastLED LED Arrays
CRGB tLeds[NUM_THROTTLE_LEDS];
CRGB bLeds[NUM_BRAKE_LEDS];

// The Arduino reads between 0 and 1023 while the ESP32 reads between 0 and 4095
// Adjusts maximum voltage levels read by the device will adjust this based on
// the total voltage output from throttle to ensure we can max out the gauge
// const int inputDivisor = 1023;  // for Arduino devices
const int inputDivisor = 4095;  // for ESP32 devices
const int brakeThreshold = inputDivisor / 2;
const float maxThrottleValue = THROTTLE_MAX * 0.9;
const float pixelRatio = (maxThrottleValue - THROTTLE_MIN) / NUM_THROTTLE_LEDS;

bool wasMaxThrottle = false;
bool wasMinThrottle = false;
bool prevBrakeState = false;

int prevPixelCount = 0;

void setup() {

  Serial.begin(115200);
  delay(1000);
  Serial.println("\nThrottle Gauge");
  Serial.println("By John M. Wargo & Scott Good\n");

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

  ledDemo();
}

void loop() {
  updateThrottle();
  updateBrake();
  delay(200);
}

void updateThrottle() {

  bool pixelStateChange;
  bool isMaxThrottle;
  bool isMinThrottle;

  int throttleValue;
  int currentPixelCount;

  // read the voltage from the throttle pin
  throttleValue = analogRead(INPUT_THROTTLE);
#ifdef DEBUG
  Serial.printf("Throttle Value: %d\n", throttleValue);
#endif

  // determine special cases
  isMaxThrottle = throttleValue > maxThrottleValue;
  isMinThrottle = throttleValue < THROTTLE_MIN;

  // Did the throttle value change at least one pixel's worth?
  currentPixelCount = (throttleValue > THROTTLE_MIN) ? (throttleValue - THROTTLE_MIN) / pixelRatio : 0;

  // did the number of illuminated pixels change?
  pixelStateChange = currentPixelCount != prevPixelCount;
  // But, are we sitting below throttleMin?
  if (isMinThrottle && wasMinThrottle) {
    // Then just leave it alone
    pixelStateChange = false;
  }
  // But, are we sitting above max throttle?
  if (isMaxThrottle && wasMaxThrottle) {
    // Then just leave it alone
    pixelStateChange = false;
  }

  // reset our previous loop values for the next check
  prevPixelCount = currentPixelCount;
  wasMaxThrottle = isMaxThrottle;
  wasMinThrottle = isMinThrottle;

  if (pixelStateChange) {
    // then the numbers of illuminated pixels changed
    if (isMaxThrottle) {
      // Set all throttle LEDs to WHITE
      for (int i = 0; i < NUM_THROTTLE_LEDS; i++) tLeds[i] = CRGB::White;
    } else {  // else not isMaxThrottle
      // first, set pixel count to zero if no throttle
      if (isMinThrottle) {
        // No pixels illuminated, this allows the code to skip the next if statement
        // then sets all pixels to black
        currentPixelCount = 0;
      }
      // Set only a portion of the throttle LEDs to Green
      if (currentPixelCount > 0) {
        // light LEDs green based on the current throttle value
        for (int i = 0; i < currentPixelCount; i++) tLeds[i] = CRGB::Green;
      }
      // then set the rest to black
      for (int i = currentPixelCount; i < NUM_THROTTLE_LEDS; i++) tLeds[i] = CRGB::Black;
    }  //if (isMaxThrottle)
    FastLED.show();
  } // if (pixelStateChange) 
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

void ledDemo() {
  Serial.println("Testing LEDs");

  FastLED.clear();
  FastLED.show();
  for (int i = 0; i < NUM_THROTTLE_LEDS; i++) {
    tLeds[i] = CRGB::Green;
    FastLED.show();
    delay(testDelay);
  }
  for (int i = NUM_THROTTLE_LEDS - 1; i > -1; i--) {
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
