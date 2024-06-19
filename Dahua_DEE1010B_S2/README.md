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
|D2(GPIO4)|Button pin|short to GND to activate button, Connect via external 4.7k Resistor to 3V3|

# Communication and frame description
VTO (eg. VTO2111D-P-S2) work as master, DEE1010B_S2 as slave. Both communicate via RS485 protocol, Bit rate 9600 Bits/s, 1 stop bit, no parity.

After reboot (power reset) of VTO (eg. VTO2111D-P-S2) bitrate pattern is transmitted by master. After this first frame "frame 3" is transmitted
![image](https://github.com/mrmdudek/Dahua-DEE1010B-S2/assets/57598064/f26f26bd-7154-4f3e-84dd-61e3b74f839c)

Next frame during initialisation 	
![image](https://github.com/mrmdudek/Dahua-DEE1010B-S2/assets/57598064/c9873d15-4ac9-497a-813f-4c244387dd99)

The previous frame is repeated	
![image](https://github.com/mrmdudek/Dahua-DEE1010B-S2/assets/57598064/68a1682b-ba19-417e-988d-818680ca1a29)

Another initialisation frame
![image](https://github.com/mrmdudek/Dahua-DEE1010B-S2/assets/57598064/40026cb3-cdc8-4ccd-a0ad-5f11813a7935)

Another initialisation frame
![image](https://github.com/mrmdudek/Dahua-DEE1010B-S2/assets/57598064/42485863-0949-4aac-8150-2ed8370137bc)

Then standard/normal communication start
![image](https://github.com/mrmdudek/Dahua-DEE1010B-S2/assets/57598064/d64ed0e3-116a-4d6c-bf1a-217b5cf3a6ad)

Next frame has counter increased
![image](https://github.com/mrmdudek/Dahua-DEE1010B-S2/assets/57598064/39420de1-6d0d-4143-99a0-a9552d8d71b0)

...
Communication frame every ~500ms
...

Let's follow door open request frame - event type frame. 

Normal frame

![image](https://github.com/mrmdudek/Dahua-DEE1010B-S2/assets/57598064/7cfde598-522e-4ad0-94b4-f23374fdb440)

Request open frame (event type)
![image](https://github.com/mrmdudek/Dahua-DEE1010B-S2/assets/57598064/b0a0455b-92bb-49a3-a5f2-7fe71f687bec)

Slave respond with additional parameters
![image](https://github.com/mrmdudek/Dahua-DEE1010B-S2/assets/57598064/270ba52b-d6a6-44ec-9d3e-c9947a013ad1)

Normal communication (with the same counter as previous, next frame increase counter)
![image](https://github.com/mrmdudek/Dahua-DEE1010B-S2/assets/57598064/10dae439-6ec5-4ce7-ba8a-57acfef1dc57)

For detailed timing and event type please check communication logging: 
![link](https://github.com/mrmdudek/Dahua-DEE1010B-S2/tree/main/Communication%20logging)

# Tests
Communication and Relay functionality tested with ESP32 (LOLIN D32) and ESP8266 (Wemos D1 mini). Tested with Dahua KTP03 (VTH2621G-P + VTO2111D-P-S2 + PFS3006-4ET-36)

# Why to use your hardware and this library instead of buy DEE1010B_S2?
Educational purpose, Interacting with other system (eg. home assistant), Cheaper

# Bugs / not working / not tested
FBK not implemented, 
button not tested, 
RS485 additional bus for card reader not implemented,
other boards not tested and not compiled

# Warranty
No warranty
