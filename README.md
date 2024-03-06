This is my clone of a Friendship lamp
I used HiveMQ for my MQTT broker (that is the public key committed in the code)
Configuration is all done though WiFiManager.h

Dependencies:
ArduinoJson by Benoit Blanchon 7.0.3
FastLED by Daniel Garcia 3.6.0
PubSubClient by Nick O'Leary 2.8
WiFiManager by tzapu 2.0.17

esp8266 board files 3.1.2


Wiring for final app:
  Button connected to 3.3V and D0 (pin 16 according to ESP internal documentation)
  LED strip connected to GND, 5V, and D1 (pin 5 acording to ESP internal documentation)

On bootup it will attempt to connect to previous wifi and MQTT broker.
If wifi connection is unsuccessful it will host a configuration page.
To force configuration hold the button while booting.

Configure the MQTT URL / username / password according to your broker
HiveMQ is kind of assumed, port is the standard MQTT secure port, certificate chain is from their broker
