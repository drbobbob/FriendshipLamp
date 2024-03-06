#include <FS.h>
#include <WiFiManager.h>

#include <ArduinoJson.h>

// #include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>

#include <TaskScheduler.h>
#include <MyTasks.h>


#include "Config.h"
#include "MainButtonTask.h"
#include "MQTTHandler.h"

#define NUM_LEDS 6
CRGB leds[NUM_LEDS];

// This is "pin 5" but labeled on the board as D1
#define LED_PIN 5

// #define BUTTON_PIN D7
// 16 is D0
#define BUTTON_PIN 16

static MQTTHandlerTask mqttTask;
static MainButtonTask buttonTask(BUTTON_PIN, mqttTask);

static Task* tasks[] = {&buttonTask, &mqttTask};

static TaskScheduler ts(tasks, NUM_TASKS(tasks));

void NTPSetup()
{
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));

}

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Serial.println();

  //resetting pinmode from button task
  pinMode(BUTTON_PIN, INPUT_PULLDOWN_16);

  WiFi.mode(WIFI_STA);

  bool configSuccess = ReadConfig();

  Serial.println(mqtt_server);
  // bool forceConfig = !digitalRead(BUTTON_PIN);
  bool forceConfig = !configSuccess;
  MyWiFiConfiguration(forceConfig);

  NTPSetup();
  mqttTask.SetupMQTTHandling();

  Serial.println("Setup is done");
  // FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
}

void loop() {
  // put your main code here, to run repeatedly:
  ts.runOnce();
}
