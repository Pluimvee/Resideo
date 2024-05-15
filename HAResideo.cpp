/*

*/

#include "HAResideo.h"
#include "Sensors.h"
#include <PolledTimeout.h>
#include <String.h>
#include <DatedVersion.h>
DATED_VERSION(0, 1)
#define DEVICE_MODEL "Resideo Mod esp8266"

////////////////////////////////////////////////////////////////////////////////////////////
#define LOG_REMOTE
#define LOG_LEVEL 2
#include <Logging.h>

////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////
#define CONSTRUCT_P0(var)       var(#var, HABaseDeviceType::PrecisionP0)
#define CONSTRUCT_P1(var)       var(#var, HABaseDeviceType::PrecisionP1)

#define CONFIGURE_BASE(var, name, class, icon)  var.setName(name); var.setDeviceClass(class); var.setIcon("mdi:" icon)
#define CONFIGURE(var, name, class, icon, unit) CONFIGURE_BASE(var, name, class, icon); var.setUnitOfMeasurement(unit)

////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////
HAResideo::HAResideo()
: CONSTRUCT_P0(co2_level), CONSTRUCT_P1(humidity), CONSTRUCT_P1(temperature)
{
  CONFIGURE(temperature,"Temperature","temperature",    "thermometer",    "°C");
  CONFIGURE(co2_level,  "CO2",        "carbon_dioxide", "molecule-co2",   "ppm");
  CONFIGURE(humidity,   "Humidity",   "humidity",       "water-percent",  "%");
}

////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////
bool HAResideo::setup(const byte mac[6], HAMqtt *mqtt) 
{
  enableExtendedUniqueIds();  
  setUniqueId(mac, 6);
  setManufacturer("InnoVeer");
  setName(DEVICE_NAME);
  setSoftwareVersion(VERSION);
  setModel(DEVICE_MODEL);

  mqtt->addDeviceType(&humidity);  
  mqtt->addDeviceType(&temperature);  
  mqtt->addDeviceType(&co2_level);  

  CHT8305::setup();
  CM1106::setup();

  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////
bool HAResideo::loop()
{
  using periodic = esp8266::polledTimeout::periodicMs;
  static periodic nextPing(1500);

  if (nextPing) {
    float T = CHT8305::temperature(), H = CHT8305::humidity();
    uint16_t P = CM1106::ppm();

    INFO("[%s] -> T:%.1f   H:%.1f   C:%.1u\n", getUniqueId(), T, H, P);

    temperature.setValue(T);
    humidity.setValue(H);
    co2_level.setValue(P);
  }
  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

