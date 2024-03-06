#include <FS.h>
#include <WiFiManager.h>

#include <ArduinoJson.h>

char mqtt_server[60];
char mqtt_port[6] = "8883";
char mqtt_user[20];
char mqtt_password[20];

// char wifi_ssid[20];
// char wifi_password[20];

bool shouldSaveConfig = false;

void saveConfigCallback()
{
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Serial.println();

  WiFi.mode(WIFI_STA);

  Serial.println("mounting FS...");

  if(SPIFFS.begin()) {
    Serial.println("mounted file system");
    if(SPIFFS.exists("/config.json"))
    {
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
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
          strcpy(mqtt_server, json["mqtt_server"]);
          strcpy(mqtt_port, json["mqtt_port"]);
          strcpy(mqtt_user, json["mqtt_user"]);
          strcpy(mqtt_password, json["mqtt_password"]);
          // strcpy(wifi_ssid, json["wifi_ssid"]);
          // strcpy(wifi_password, json["wifi_password"]);
        }
        else
        {
          Serial.println("failed to load json config");
        }
        configFile.close();
      }
    }
  }
  else {
    Serial.println("failed to mount FS");
  }

  WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 60);
  WiFiManagerParameter custom_mqtt_port("port", "mqtt port", mqtt_port, 6);
  WiFiManagerParameter custom_mqtt_username("username", "mqtt username", mqtt_user, 20);
  WiFiManagerParameter custom_mqtt_password("password", "mqtt password", mqtt_password, 20);
  WiFiManagerParameter custom_field("ledColor", "LED Color", "#000000", 8, "type='color'");

  WiFiManager wifiManager;

  wifiManager.resetSettings();

  wifiManager.setSaveConfigCallback(saveConfigCallback);
  wifiManager.setSTAStaticIPConfig(IPAddress(10,0,1,99), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.addParameter(&custom_mqtt_port);
  wifiManager.addParameter(&custom_mqtt_username);
  wifiManager.addParameter(&custom_mqtt_password);

  //   const char* custom_radio_str = "<br/><label for='customfieldid'>Custom Field Label</label><input type='color' id='customfieldid' name='customfieldid'>";
  // WiFiManagerParameter custom_field(custom_radio_str); // custom html input


  wifiManager.addParameter(&custom_field);



  if(!wifiManager.autoConnect("AutoConnectAP", "password")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);

    ESP.restart();
    delay(5000);
  }

  strcpy(mqtt_server, custom_mqtt_server.getValue());
  strcpy(mqtt_port, custom_mqtt_port.getValue());
  strcpy(mqtt_user, custom_mqtt_username.getValue());
  strcpy(mqtt_password, custom_mqtt_password.getValue());

  Serial.print("Custom color value: ");
  Serial.println(custom_field.getValue());

  if(shouldSaveConfig) {
    Serial.println("saving config");
    DynamicJsonDocument json(1024);
    json["mqtt_server"] = mqtt_server;
    json["mqtt_port"] = mqtt_port;
    json["mqtt_user"] = mqtt_user;
    json["mqtt_password"] = mqtt_password;

    File configFile = SPIFFS.open("/config.json", "w");
    if(!configFile){
      Serial.println("failed to open config file for writing");
    }
    serializeJson(json, Serial);
    serializeJson(json, configFile);
    configFile.close();
  }

}

void loop() {
  // put your main code here, to run repeatedly:

}
