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

// task handlers for the code running on separate processor cores
TaskHandle_t Task0;
TaskHandle_t Task1;

CRGB leds[NUM_LEDS];

int throttleValue = 0;
int previousThrottleValue = -1;

void setup() {

  Serial.begin(115200);
  delay(1000);
  Serial.println("\nThrottle Gauge");
  Serial.println("By John M. Wargo\n");

  // make sure we have an appropriate amount of NeoPixels to make a gauge
  if (NUM_LEDS < 5) {
    Serial.println("Invalid Configuration");
    Serial.println("Number of NUM_LEDS must be greater than 5");
    while (true) delay(100);  //loops forever
  }

  // initialize the FastLED library
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  // turn off all LEDs
  fill_solid(leds, NUM_LEDS, CRGB::Black);

  // Pin the throttle reader to processor core 0
  xTaskCreatePinnedToCore(throttleReader, "Throttle Reader", 10240, NULL, 0, &Task0, 0);
  // give the processor some time to get the thread running
  delay(500);
  // Pin the gauge updater to processor core 1
  xTaskCreatePinnedToCore(gaugeUpdater, "Gauge Updater", 10240, NULL, 0, &Task1, 1);
  // give the processor some time to get the thread running
  delay(500);
}

void loop() {
  // nothing to do here since we have dedicated tasks
  // running on each core looping away infinitely
}

void throttleReader(void* pvParameters) {

  Serial.print("Throttle Reader running on core ");
  Serial.println(xPortGetCoreID());

  for (;;) {  // an infinite loop
    // read the voltage from the throttle pin, returns values from 0 to 1023
    throttleValue = analogRead(THROTTLE_PIN);
    // throw in a little delay for the ESP32 watchdog to do its cleanup
    // https://randomerrors.dev/posts/2023/esp32-watchdog-got-triggered/
    delay(25);
  }
}

void gaugeUpdater(void* pvParameters) {

  int illuminatedPixels;

  Serial.print("Gauge Updater running on core ");
  Serial.println(xPortGetCoreID());

  for (;;) {  // an infinite loop
    // Did the throttle value change?
    if (throttleValue != previousThrottleValue) {
      // reset the previous throttle value
      previousThrottleValue = throttleValue;
      // update the gauge
      // convert the voltage to a number of NeoPixels
      // TODO: Scale this value a bit so we can achieve full throttle
      illuminatedPixels = int(throttleValue / 1023);
      // turn off all of the LEDs; this may happen fast enough to not be visible
      // otherwise I may have to do this manually, we'll see
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      // light the green ones based on the throttle value
      fill_solid(leds, throttleValue, CRGB::Green);
    }
    // throw in a little delay for the ESP32 watchdog to do its cleanup
    // https://randomerrors.dev/posts/2023/esp32-watchdog-got-triggered/
    delay(25);
  }
}