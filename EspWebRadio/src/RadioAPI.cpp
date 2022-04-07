#include "RadioAPI.h"

WebServer *RadioAPI::webserver = NULL;
Radio *RadioAPI::radio = NULL;
RadioChannel RadioAPI::currentChannel = {};
RadioChannel RadioAPI::currentAlarmChannel = {};
RadioAlarm *RadioAPI::alarm = NULL;

RadioAPI::RadioAPI(WebServer *webserver, Radio *radio, RadioAlarm *alarm)
{

  RadioAPI::webserver = webserver;
  RadioAPI::radio = radio;
  RadioAPI::alarm = alarm;
  currentChannel = {"startUPChannel", "http://liveradio.swr.de/sw282p3/swr3"};
  currentAlarmChannel = {"startUPChannel", "http://liveradio.swr.de/sw282p3/swr3"};

  webserver->on("/", onRoot);
  webserver->on("/channel", onChannel);
  webserver->on("/play", onPlay);
  webserver->on("/stop", onStop);
  webserver->on("/volume", onVolume);
  webserver->on("/alarm", onAlarm);
  webserver->on("/snooze", onSnooze);
  webserver->on("/stop-alarm", onStopAlarm);
  webserver->onNotFound(onNotFound);
}

void RadioAPI::onRoot()
{
  webserver->send(200, "text/html", webpage);
  if (!radio->isPlaying())
  {
    radio->say("You made it! Just choose your favorite station and give it a go. Or set an Alarm if you want.");
  }
}

void RadioAPI::onNotFound()
{
  webserver->send(404);
}

void RadioAPI::onChannel()
{
  if (webserver->hasArg("url"))
  {
    currentChannel = {"custom", webserver->arg("url")};
    Serial.println("CurrentChannel: " + currentChannel.name + "  " + currentChannel.url);
    radio->play(currentChannel);
  }
  webserver->send(200);
}

void RadioAPI::onPlay()
{
  Serial.println("CurrentChannel: " + currentChannel.name + "  " + currentChannel.url);
  radio->play(currentChannel);

  webserver->send(200);
}

void RadioAPI::onStop()
{
  radio->stop();
  webserver->send(200);
}

void RadioAPI::onVolume()
{
  String volumeString = webserver->arg("volume");
  uint8_t volume = volumeString.toInt();
  radio->setVolume(volume);
  webserver->send(200);
}

void RadioAPI::onAlarm()
{
  String timeString = webserver->arg("time");
  struct tm time = {0};
  int year, month, day, hour, minute, second;
  sscanf(timeString.c_str(), "%d-%d-%dT%d:%d:%dZ", &year, &month, &day, &hour, &minute, &second);
  time.tm_year = year - 1900;
  time.tm_mon = month - 1;
  time.tm_mday = day;
  time.tm_hour = hour;
  time.tm_min = minute;
  time.tm_sec = second;
  time.tm_isdst = -1;
  alarm->setAlarm(time);

  String snooze = webserver->arg("snooze");
  uint8_t snoozeMinutes = snooze.toInt();
  alarm->setSnooze(snoozeMinutes);

  String channel = webserver->arg("url");
  String url = webserver->arg("url");
  currentAlarmChannel = {"custom", url.c_str()};
  alarm->setChannel(currentAlarmChannel);
  alarm->start();
  webserver->send(200);
}

void RadioAPI::onSnooze()
{
  if (!alarm->alarmIsSet() && !alarm->snoozeIsSet())
  {
    alarm->snooze();
  }
  webserver->send(200);
}

void RadioAPI::onStopAlarm()
{
  alarm->stop();
  webserver->send(200);
}
