#include "LEDManager.h"

#include <Adafruit_NeoPixel.h>

#define NUM_LEDS 6

// This is "pin 5" but labeled on the board as D1
#define LED_PIN 5


Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void InitLEDs()
{
  strip.begin();
  strip.setBrightness(100);
}

void SetLEDColor(const char * payload)
{
  char buffer[7];
  buffer[6] = 0;

  buffer[0] = payload[1];
  buffer[1] = payload[2];
  buffer[2] = payload[3];
  buffer[3] = payload[4];
  buffer[4] = payload[5];
  buffer[5] = payload[6];

  uint32_t newColor = strtoul(buffer, 0, 16);
  for(int i = 0; i < NUM_LEDS; i++)
  {
    strip.setPixelColor(i, newColor);
  }
  strip.show();
}