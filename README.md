# Resideo Mod 
Resideo Mod to connect to Home Automation via MQTT 
This code is running on a WEMOS P8266
- Reading CO2 ppm via RX
- Reading Humidity and temperature by sniffing the I2C

![20240414_113854](https://github.com/Pluimvee/Resideo/assets/124380379/8b7ecabb-fb0e-4d0d-8693-c968d05f2e3c)

Research on CHT8305 Humidity/Temperature sensor by sniffing I2C using
https://github.com/Pluimvee/I2C-sniffer/blob/main/I2C-sniffer.ino

Research on CM1106 sensor by connecting TTL-2-USB bridge

My graditude to the research and work of the following tweakers:
- Bartvb (https://gathering.tweakers.net/forum/list_message/78228570#78228570)
- Soepstengel (https://gathering.tweakers.net/forum/list_message/77882454#77882454)
- Skix_Aces (https://gathering.tweakers.net/forum/list_message/78427188#78427188)
- ThinkPad (https://tweakers.net/gallery/146731/)

Uses the Home Assistant MQTT Library (https://github.com/dawidchyrzynski/arduino-home-assistant)

![20240413_135641](https://github.com/Pluimvee/Resideo/assets/124380379/2ebb0d7c-32a8-4dca-b83b-b1bceb610ddd)
![20240412_132045](https://github.com/Pluimvee/Resideo/assets/124380379/a236c325-c32b-455f-8b65-0edd9c022154)
![20240411_154721](https://github.com/Pluimvee/Resideo/assets/124380379/951a8c7a-39ac-4cd2-afea-eef955254ee9)
![20240410_204245](https://github.com/Pluimvee/Resideo/assets/124380379/baf13532-3136-48af-aa4b-726a544452a0)
