#include <Audio.h>
#include <WebServer.h>
#include <AutoConnect.h> //wifi AutoConnect lib
#include <Wifi.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h> //OTA Update

#include "Radio.h"
#include "RadioAPI.h"
#include "RadioAlarm.h"

#define AUTO_CONNECT_AP "WebRadio"
#define AUTO_CONNECT_PW ""
#define AUTO_CONNECT_HOST "WebRadio WiFi"
#define LOCAL_DOMAIN_NAME "webradio"

#define I2S_DOUT 25
#define I2S_BCLK 27
#define I2S_LRC 26
#define AMP_ENABLE 33

WebServer server;                                                      // define server instance
AutoConnect portal(server);                                            // create AutoConnect portal
AutoConnectConfig configAutoConnect(AUTO_CONNECT_AP, AUTO_CONNECT_PW); // define AutoConnectConfig instance

Audio audio;

Radio *radio;
RadioAPI *api;
RadioAlarm *radioAlarm;

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting up ...");

  radio = new Radio(&audio, AMP_ENABLE); // create radio, API and alarm instance
  radioAlarm = new RadioAlarm(radio);
  api = new RadioAPI(&server, radio, radioAlarm);

  while (!audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT))
  {
    Serial.println("[Error] Couldn't start audio player!");
    delay(5000);
  }

  WiFi.setHostname(AUTO_CONNECT_HOST);            // define hostname in dhcp mode
  configAutoConnect.hostName = AUTO_CONNECT_HOST; // define hostname in ap mode
  portal.config(configAutoConnect);               // update Portal with new AP name and PW

  if (portal.begin())
  { // start AutoConnect process
    Serial.println("WiFi connected: " + WiFi.localIP().toString());
  }

  //##################OTA Setup###################################

  // OTA Hostname (shows in Arduino IDE)
  ArduinoOTA.setHostname(AUTO_CONNECT_HOST);
  ArduinoOTA.onStart([]()
                     {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type); })
      .onEnd([]()
             { Serial.println("\nEnd"); })
      .onProgress([](unsigned int progress, unsigned int total)
                  { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); })
      .onError([](ota_error_t error)
               {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed"); });
  //##################OTA Setup###################################end

  ArduinoOTA.begin(); // start OTA

  radioAlarm->init();
}

void loop()
{
  portal.handleClient(); // handle AutoConnect client

  if (!radio->isPlaying())
  {
    ArduinoOTA.handle();   // handle OTA
    radioAlarm->handler(); // compare time, fire alarm, fire snooze
  }
  audio.loop(); // handle the tune
}
