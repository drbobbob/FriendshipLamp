#include <FS.h>
#include <WiFiManager.h>
#include <ArduinoJson.h>


#define CONFIG_PATH "/config2.json"

#define MQTT_SERVER_JSON "mqtt_server"
#define MQTT_PORT_JSON "mqtt_port"
#define MQTT_USER_JSON "mqtt_user"
#define MQTT_PASSWORD_JSON "mqtt_password"
#define LED_COLOR_JSON "led_color"
#define MQTT_TOPIC_PREFIX_JSON "mqtt_topic"

#define MQTT_SERVER_LEN 60
#define MQTT_AUTH_LEN 20
#define COLOR_STR_LEN 8
#define MQTT_TOPIC_PREFIX_LEN 20

char mqtt_server[MQTT_SERVER_LEN] = "none";
char mqtt_user[MQTT_AUTH_LEN] = "none";
char mqtt_password[MQTT_AUTH_LEN] = "none";
char my_led_color[COLOR_STR_LEN] = "#00ff00";
char mqtt_topic_prefix[MQTT_TOPIC_PREFIX_LEN] = "none";

bool shouldSaveConfig = false;

bool ReadConfig()
{
  bool retVal = false;
  Serial.println("mounting FS...");

  if(SPIFFS.begin()) {
    Serial.println("mounted file system");
    if(SPIFFS.exists(CONFIG_PATH))
    {
      Serial.println("reading config file");
      File configFile = SPIFFS.open(CONFIG_PATH, "r");
      if(configFile){
        Serial.println("opened config file");
        size_t size = configFile.size();
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);

        DynamicJsonDocument json(1024);
        auto deserializerError = deserializeJson(json, buf.get());
        serializeJson(json, Serial);
        if(!deserializerError)
        {
          Serial.println("\nparsed json");
          strcpy(mqtt_server, json[MQTT_SERVER_JSON]);
          strcpy(mqtt_user, json[MQTT_USER_JSON]);
          strcpy(mqtt_password, json[MQTT_PASSWORD_JSON]);
          strcpy(my_led_color, json[LED_COLOR_JSON]);
          strcpy(mqtt_topic_prefix, json[MQTT_TOPIC_PREFIX_JSON]);
          retVal = true;
        }
        else
        {
          Serial.println("failed to load json config");
        }
        configFile.close();
      }
    }
    else
    {
      Serial.println("No config file");
    }
  }
  else {
    Serial.println("failed to mount FS");
  }
  return retVal;
}

void SaveConfig()
{
    Serial.println("saving config");
    DynamicJsonDocument json(1024);
    json[MQTT_SERVER_JSON] = mqtt_server;
    json[MQTT_USER_JSON] = mqtt_user;
    json[MQTT_PASSWORD_JSON] = mqtt_password;
    json[LED_COLOR_JSON] = my_led_color;
    json[MQTT_TOPIC_PREFIX_JSON] = mqtt_topic_prefix;

    File configFile = SPIFFS.open(CONFIG_PATH, "w");
    if(!configFile){
      Serial.println("failed to open config file for writing");
    }
    serializeJson(json, Serial);
    serializeJson(json, configFile);
    configFile.close();
}

void saveConfigCallback()
{
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

void MyWiFiConfiguration(bool forceConfig)
{

  WiFiManager wifiManager;

  if(forceConfig)
  {
    wifiManager.resetSettings();
  }

  wifiManager.setSaveConfigCallback(saveConfigCallback);
  wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,99), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

  WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, MQTT_SERVER_LEN);
  wifiManager.addParameter(&custom_mqtt_server);
  WiFiManagerParameter custom_mqtt_username("username", "mqtt username", mqtt_user, MQTT_AUTH_LEN);
  wifiManager.addParameter(&custom_mqtt_username);
  WiFiManagerParameter custom_mqtt_password("password", "mqtt password", mqtt_password, MQTT_AUTH_LEN);
  wifiManager.addParameter(&custom_mqtt_password);
  WiFiManagerParameter custom_mqtt_topic_prefix("topicPrefix", "MQTT Topic Prefix", mqtt_topic_prefix, MQTT_TOPIC_PREFIX_LEN);
  wifiManager.addParameter(&custom_mqtt_topic_prefix);
  WiFiManagerParameter custom_led_color("ledColor", "LED Color", my_led_color, COLOR_STR_LEN, "type='color'");
  wifiManager.addParameter(&custom_led_color);

  if(!wifiManager.autoConnect("FriendshipLampAP", "password")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    ESP.restart();
    delay(5000);
  }

  strcpy(mqtt_server, custom_mqtt_server.getValue());
  strcpy(mqtt_user, custom_mqtt_username.getValue());
  strcpy(mqtt_password, custom_mqtt_password.getValue());
  strcpy(mqtt_topic_prefix, custom_mqtt_topic_prefix.getValue());
  strcpy(my_led_color, custom_led_color.getValue());

  if(shouldSaveConfig) {
    SaveConfig();
  }
}