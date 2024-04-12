#include <ArduinoHA.h>
#include <ArduinoHADefines.h>
#include <HADevice.h>
#include <HAMqtt.h>

#ifndef HOME_ASSIST_RESIDEO
#define HOME_ASSIST_RESIDEO

#include <HADevice.h>
#include <device-types\HASensorNumber.h>
#define SENSOR_COUNT 5   

////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////
class HAResideo : public HADevice 
{
private:
//  OneWire            _wire;
public:
  HAResideo();

  HASensorNumber  temperature;
  HASensorNumber  humidity;
  HASensorNumber  co2_level;
  
  bool begin(const byte mac[6], HAMqtt *mqqt);
  bool loop();                                                    
};

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

#endif