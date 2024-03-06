
#pragma once

extern char* mqtt_server;
extern char* mqtt_port;
extern char* mqtt_user;
extern char* mqtt_password;
extern char* my_led_color;
extern char* mqtt_topic_prefix;

void ReadConfig();
void WiFiConfiguration(bool forceConfig);
