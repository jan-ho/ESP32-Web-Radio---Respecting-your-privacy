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

#define CHANNEL_COUNT 7

RadioChannel channels[CHANNEL_COUNT] = {
    {"SWR1", "http://liveradio.swr.de/sw282p3/swr1bw"},                // SWR1
    {"SWR3", "http://liveradio.swr.de/sw282p3/swr3"},                  // SWR3
    {"SWR4", "http://liveradio.swr.de/sw282p3/swr4bw"},                // SWR4
    {"Antenne 1", "http://stream.antenne1.de/a1stg/livestream2.mp3"},  // Antenne 1
    {"Antenne Bayern", "http://stream.antenne.de/antenne/stream/mp3"}, // Antenne Bayern
    {"BigFM", "http://streams.bigfm.de/bigfm-deutschland-128-mp3"},    // BigFM
    {"107.7", "http://addrad.io/4454xb3"}                              // 107.7
};

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
  api = new RadioAPI(&server, radio, channels, CHANNEL_COUNT, radioAlarm);

  MDNS.begin(LOCAL_DOMAIN_NAME);                  // start mDNS responder for local name
  WiFi.setHostname(AUTO_CONNECT_HOST);            // define hostname in dhcp mode
  configAutoConnect.hostName = AUTO_CONNECT_HOST; // define hostname in ap mode
  portal.config(configAutoConnect);               // update Portal with new AP name and PW

  if (portal.begin())
  { // start AutoConnect process
    Serial.println("WiFi connected: " + WiFi.localIP().toString());
  }
  MDNS.addService("http", "tcp", 80); // add HTTP service to mDNS responder

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

  while (!audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT))
  {
    Serial.println("[Error] Couldn't start audio player!");
    delay(5000);
  }
  radioAlarm->init();
}

void loop()
{
  portal.handleClient(); // handle AutoConnect client
  ArduinoOTA.handle();   // handle OTA
  audio.loop();          // handle the tune
  radioAlarm->handler(); // compare time, fire alarm, fire snooze
}
