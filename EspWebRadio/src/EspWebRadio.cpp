#include <Audio.h>
#include <WebServer.h>
#include <AutoConnect.h> //wifi AutoConnect lib
#include <WiFi.h>

#include "defines.h"
#include "Radio.h"
#include "RadioAPI.h"
#include "RadioAlarm.h"
#include "Bluetooth.h"



WebServer server;                                                      // define server instance
AutoConnect *portal;                                            // create AutoConnect portal
AutoConnectConfig configAutoConnect(AUTO_CONNECT_AP, AUTO_CONNECT_PW); // define AutoConnectConfig instance

Audio *audio;

Bluetooth *bluetooth;


Radio *radio;
RadioAPI *api;
RadioAlarm *radioAlarm;

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting up ...");

  audio = new Audio();
  radio = new Radio(audio); // create radio, API and alarm instance
  radioAlarm = new RadioAlarm(radio);
  portal = new AutoConnect(server);
  bluetooth = new Bluetooth(portal);
  api = new RadioAPI(&server, radio, radioAlarm, bluetooth);
  

  while (!audio->setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT))
  {
    Serial.println("[Error] Couldn't start audio player!");
    delay(5000);
  }

  WiFi.setHostname(AUTO_CONNECT_HOST);            // define hostname in dhcp mode
  configAutoConnect.hostName = AUTO_CONNECT_HOST; // define hostname in ap mode
  configAutoConnect.ota = AC_OTA_BUILTIN;         // enable OTA with autoconnect
  portal->config(configAutoConnect);               // update Portal with new AP name and PW

  if (portal->begin())
  { // start AutoConnect process
    String buffer = "Connected under IP:" + WiFi.localIP().toString();
    #ifdef NOTSOPRIVATE
    radio->say(buffer.c_str());
    #endif
  }


  radioAlarm->init();
}

void loop()
{
  portal->handleClient(); // handle AutoConnect client
  radioAlarm->handler(); // compare time, fire alarm, fire snooze
  audio->loop(); // handle the tune
}
