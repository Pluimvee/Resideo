# Resideo Mod
Resideo Mod to connect to Home Automation via MQTT 
This code is running on an ESP8266 module ESP-12F
- Reading CO2 ppm via RX
- Reading Humidity and temperature by sniffing the I2C

![image](https://github.com/Pluimvee/Resideo/assets/124380379/6a76a810-f088-4fe5-a577-e125a59cacdd)

# Dependencies
Uses the Home Assistant MQTT Library 
- https://github.com/dawidchyrzynski/arduino-home-assistant

Some private helpers
- https://github.com/Pluimvee/Pluimvee-ArduinoLib/blob/master/src/DatedVersion.h
- https://github.com/Pluimvee/Pluimvee-ArduinoLib/blob/master/src/Logging.h

# Hardware
- 4x Honeywell voor €84 (inc. shipment) https://www.ibood.com/nl/s-nl/o/2x-honeywell-resideo-co2-monitor-detector/987304
- 4x ESP-12F voor €6 (inc. shipment) https://nl.aliexpress.com/item/4001157391459.html?spm=a2g0o.cart.0.0.5c4c61d7SvafSV&mp=1&gatewayAdapt=glo2nld

So in total €23 per unit

# Wiring
The wiring of the ESP8266 ESP12F module to the Honeywell Resideo

![image](https://github.com/Pluimvee/Resideo/assets/124380379/e06723f1-a673-4f59-8667-7b77084dd7cb)

# Research
Sniffing the communication of the CHT8305 Humidity/Temperature sensor I used the below code. This code is using interrupts for more stability

https://github.com/Pluimvee/I2C-sniffer/blob/main/I2C-sniffer.ino

The SDA/SCL pins can also be found on the display-board-connector, and on the backside of the display-board. I tried to solder some pins on the backside of the display-board however the test-pads are to week to hold any pins. Therefore I soldered some pins into the pads next to the battery connector.

Sniffing the communication of the CM1106 sensor I used  by connecting TTL-2-USB bridge
https://nl.aliexpress.com/item/1005002007754292.html?spm=a2g0o.order_list.order_list_main.102.21ef79d2GsXXQd&gatewayAdapt=glo2nld

Using this module I  found out that the pins TX and RX on the CM1106 module have the same data stream as found on the D and C pins behind the display module. 
- The C has the same data as found on the RX pin of the CO2 module. Therefore I think C stands for Command and the instruction send to the CO2 module is found -> 11 01 01 ED
- The D has the same data as found on the RX pin of the CO2 module. I asume D stands for Data and there the response of the module is found -> 16 05 01 DF1-DF4 CS

So there is no need to connect to the pins of the CM1106 module, just connect the D(ata) pin to the RX of your ESP

![image](https://github.com/Pluimvee/Resideo/assets/124380379/266c5ccd-abe3-4957-84f4-51ea9856ff9a)

My graditude to the research and work of the following tweakers:
- Bartvb (https://gathering.tweakers.net/forum/list_message/78228570#78228570)
- Soepstengel (https://gathering.tweakers.net/forum/list_message/77882454#77882454)
- Skix_Aces (https://gathering.tweakers.net/forum/list_message/78427188#78427188)
- ThinkPad (https://tweakers.net/gallery/146731/)

 
![20240413_135641](https://github.com/Pluimvee/Resideo/assets/124380379/2ebb0d7c-32a8-4dca-b83b-b1bceb610ddd)
![20240412_132045](https://github.com/Pluimvee/Resideo/assets/124380379/a236c325-c32b-455f-8b65-0edd9c022154)
![20240411_154721](https://github.com/Pluimvee/Resideo/assets/124380379/951a8c7a-39ac-4cd2-afea-eef955254ee9)
![20240410_204245](https://github.com/Pluimvee/Resideo/assets/124380379/baf13532-3136-48af-aa4b-726a544452a0)
