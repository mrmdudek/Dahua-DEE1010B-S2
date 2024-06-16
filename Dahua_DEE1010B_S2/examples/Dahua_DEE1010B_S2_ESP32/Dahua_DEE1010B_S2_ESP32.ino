#include <Dahua_DEE1010B_S2.h>
#include <Arduino.h>

Dahua_DEE1010B_S2 dahua;

void setup() {
  Serial2.begin(9600, SERIAL_8N1, 17, 16);  // ESP32 - tested on LOLIN D32 - RX pin is 17 - TX pin is 16
  dahua.begin(&Serial2, 2, 3); // ESP32 - tested on LOLIN D32 - pin 2 is relay pin - pin 3 is button pin
}

void loop() {
  dahua.Iterate();
}