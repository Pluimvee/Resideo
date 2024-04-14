#include "HAResideo.h"
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include "secrets.h"
#include <DatedVersion.h>
DATED_VERSION(0, 1)

////////////////////////////////////////////////////////////////////////////////////////////
// Configuration
const char* sta_ssid      = STA_SSID;
const char* sta_pswd      = STA_PASS;

const char* mqtt_server   = "192.168.2.170";   // test.mosquitto.org"; //"broker.hivemq.com"; //6fee8b3a98cd45019cc100ef11bf505d.s2.eu.hivemq.cloud";
int         mqtt_port     = 1883;             // 8883;
const char* mqtt_user     = MQTT_USER;
const char *mqtt_passwd   = MQTT_PASS;

////////////////////////////////////////////////////////////////////////////////////////////
// Global instances
WiFiClient        socket;
HAResideo         resideo;
HAMqtt            mqtt(socket, resideo, SENSOR_COUNT);  // Home Assistant MTTQ

////////////////////////////////////////////////////////////////////////////////////////////
// For remote logging the log include needs to be after the global MQTT definition
#define LOG_REMOTE
#define LOG_LEVEL 2
#include <Logging.h>

void LOG_CALLBACK(char *msg) { 
  LOG_REMOVE_NEWLINE(msg);
  mqtt.publish("Resideo/log", msg, true); 
}

////////////////////////////////////////////////////////////////////////////////////////////
// Connect to the STA network
void wifi_connect() 
{ 
  if (((WiFi.getMode() & WIFI_STA) == WIFI_STA) && WiFi.isConnected())
    return;

  DEBUG("Wifi connecting to %s.", sta_ssid);
  WiFi.begin(sta_ssid, sta_pswd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    DEBUG(".");
  }
  DEBUG("\n");
  INFO("WiFi connected with IP address: %s\n", WiFi.localIP().toString().c_str());
}

////////////////////////////////////////////////////////////////////////////////////////////
// MQTT Connect
void mqtt_connect() {
  INFO("Resideo v%s saying hello\n", VERSION);
}

///////////////////////////////////////////////////////////////////////////////////////
// Main Setup 
///////////////////////////////////////////////////////////////////////////////////////
void setup() 
{
  INFO("\nResideo CO2 sensor\n");
  wifi_connect();
  // start MQTT to enable remote logging asap
  INFO("Connecting to MQTT server %s\n", mqtt_server);
  uint8_t mac[6];
  WiFi.macAddress(mac);
  resideo.setup(mac, &mqtt);              // 5) make sure the device gets a unique ID (based on mac address)
  mqtt.onConnected(mqtt_connect);           // register function called when newly connected
  mqtt.begin(mqtt_server, mqtt_port, mqtt_user, mqtt_passwd);  // 

  INFO("Initialize OTA\n");
  ArduinoOTA.setPort(8266);
  ArduinoOTA.setHostname("ResideoMod");
  ArduinoOTA.setPassword(OTA_PASS);

  ArduinoOTA.onStart([]() {
    INFO("Starting remote software update");
  });
  ArduinoOTA.onEnd([]() {
    INFO("Remote software update finished");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
  });
  ArduinoOTA.onError([](ota_error_t error) {
    ERROR("Error remote software update");
  });
  ArduinoOTA.begin();
  INFO("Setup complete\n\n");
}

///////////////////////////////////////////////////////////////////////////////////////
// Main loop
///////////////////////////////////////////////////////////////////////////////////////
void loop() 
{
  // ensure we are still connected (STA-mode)
  wifi_connect();
  // handle any OTA requests
  ArduinoOTA.handle();
  // handle MQTT
  mqtt.loop();
  // handle device funcitonality
  resideo.loop();
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
