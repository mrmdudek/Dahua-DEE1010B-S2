# About Dahua_DEE1010B_S2 Arduino library
The Dahua_DEE1010B_S2 Arduino library can be used to simulate the Dahua DEE1010B_S2 device in a circuit with a VTO (e.g., VTO2111D-P-S2).

# Connecting an Arduino to a Dahua_DEE1010B_S2
Connect arduino UART via RS485 tranceiver to RS485 BUS. Flash the board. 

These instructions are for an ESP8266 Wemos D1 mini.

|ESP8266 Wemos D1 mini|RS485 converter|Notes|
|-------|------|-----|
|GND    |GND| |
|3.3V|VCC| |
|TX|RX| |
|RX|TX| |

|ESP8266 Wemos D1 mini|Button/Relay|Notes|
|-------|------|-----|
|GND    |GND| |
|3.3V|VCC| |
|D4(GPIO2)|Relay pin|Active high|
|D2(GPIO4)|Button pin|short to GND to activate button|

# Tests
Communication and Relay functionality tested with ESP32 (LOLIN D32) and ESP8266 (Wemos D1 mini). Tested with Dahua KTP03 (VTH2621G-P + VTO2111D-P-S2 + PFS3006-4ET-36)

# Why to use your hardware and this library instead of buy DEE1010B_S2?
Educational purpose, Interacting with other system (eg. home assistant), Cheaper

# Bugs / not working
FBK not implemented
button not tested
RS485 additional bus for card reader not implemented
