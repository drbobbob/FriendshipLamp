#include <FS.h>
#include <WiFiManager.h>

#include <ArduinoJson.h>

// #include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define FASTLED_ESP8266_RAW_PIN_ORDER
#include "FastLED.h"


#include "Config.h"

#define NUM_LEDS 6
CRGB leds[NUM_LEDS];

// This is "pin 5" but labeled on the board as D1
#define LED_PIN 5

// #define BUTTON_PIN D7
// 16 is D0
#define BUTTON_PIN 16

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Serial.println();
  pinMode(BUTTON_PIN, INPUT_PULLDOWN_16);

  WiFi.mode(WIFI_STA);

  ReadConfig();
  bool forceConfig = !digitalRead(BUTTON_PIN);
  WiFiConfiguration(forceConfig);

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
}

void loop() {
  // put your main code here, to run repeatedly:

}
