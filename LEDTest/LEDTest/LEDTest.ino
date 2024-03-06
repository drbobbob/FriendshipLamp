#define FASTLED_ESP8266_RAW_PIN_ORDER

#include "FastLED.h"

#define NUM_LEDS 6
CRGB leds[NUM_LEDS];

// This is "pin 5" but labeled on the board as D1
#define LED_PIN 5

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB ( 0, 0, 255);
    FastLED.show();
    delay(40);
  }
  for (int i = NUM_LEDS-1; i >= 0; i--) {
    leds[i] = CRGB ( 255, 0, 0);
    FastLED.show();
    delay(40);
  }
}
