#include <Dahua_DEE1010B_S2.h>
#include <Arduino.h>

Dahua_DEE1010B_S2 dahua;

void setup() {
  Serial.begin(9600, SERIAL_8N1);  // ESP8266 - tested on WEMOS D1 mini - TX,RX pins
  dahua.begin(&Serial, 2, 4);      // ESP8266 - tested on WEMOS D1 mini - pin 2 is relay pin - pin 4 is button pin
}

void loop() {
  dahua.Iterate();
}