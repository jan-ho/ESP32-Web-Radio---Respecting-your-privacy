#ifndef RADIOAPI_H
#define RADIOAPI_H

#include <WebServer.h>

#include "Radio.h"
#include "RadioAlarm.h"
#include "Webpage.h"
#include "Bluetooth.h"

class RadioAPI
{
public:
  RadioAPI(WebServer *webserver, Radio *radio, RadioAlarm *alarm, Bluetooth *bluetooth);

private:
  static WebServer *webserver;
  static Radio *radio;
  static RadioChannel currentChannel;
  static RadioChannel currentAlarmChannel;
  static RadioAlarm *alarm;
  static boolean *bt;
  static Bluetooth *bluetooth;

  static void onRoot();
  static void onNotFound();

  static void onChannel();
  static void onPlay();
  static void onPause();
  static void onStop();
  static void onVolume();
  static void onAlarm();
  static void onSnooze();
  static void onStopAlarm();
  static void onBluetooth();
};

#endif // RADIOAPI_H
