#include "RadioAlarm.h"

RadioAlarm::RadioAlarm(Radio *radio)
{
  this->radio = radio;
}

// init the Alarm with NTP Data
void RadioAlarm::init()
{
  configTzTime(timeZoneInfo, ntpServer);
}

// activate the Alarm
void RadioAlarm::start()
{

  activated = true;
  Serial.println("Alarm activated");
}

// deactivate the Alarm
void RadioAlarm::stop()
{
  radio->stop();
  activated = false;
  snoozed = false;
  Serial.println("Alarm deactivated");
}

// stop the alarm and go into 10 min snoozing
void RadioAlarm::snooze()
{
  radio->stop();

  if (!getLocalTime(&snoozedTime))
  {
    Serial.println("Failed to obtain time");
    return;
  }
  snoozed = true;
  Serial.println("Snoozing set");
}

// set the channel for the alarm
void RadioAlarm::setChannel(RadioChannel channel)
{
  this->channel = channel;
}

// define the alarm time
void RadioAlarm::setAlarm(struct tm wakeUpTime)
{
  this->wakeUpTime = wakeUpTime;
  Serial.println("Alarm set");
}

// Setter for snooze minutes
void RadioAlarm::setSnooze(uint8_t snoozeMinutes)
{
  this->snoozeMinutes = snoozeMinutes;
}

// alarm handler, run this in loop()
void RadioAlarm::handler()
{
  if (!getLocalTime(&localTime))
  {
    Serial.println("Failed to obtain time");
    return;
  }

  if (activated)
  {
    if (difftime(mktime(&wakeUpTime), mktime(&localTime)) == 0)
    {
      Serial.println("Alarm fired");
      activated = false;
      radio->play(channel);
    }
  }
  else if (snoozed)
  {
    if (difftime(mktime(&localTime), mktime(&snoozedTime)) >= (snoozeMinutes * 60))
    {
      Serial.println("Snoozing fired");
      snoozed = false;
      radio->play(channel);
    }
  }
}

// return state of alarm
boolean RadioAlarm::alarmIsSet()
{
  return activated;
}

// return state of snooze
boolean RadioAlarm::snoozeIsSet()
{
  return snoozed;
}
