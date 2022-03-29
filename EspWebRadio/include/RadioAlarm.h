#ifndef RADIOALARM_H
#define RADIOALARM_H

#include "Radio.h"
#include "time.h"

#define DEFAULT_SNOOZE_MIN 10

class RadioAlarm
{
public:
  RadioAlarm(Radio *radio);

  void init();
  void start();
  void stop();
  void snooze();
  void setChannel(RadioChannel channel);
  void setAlarm(struct tm wakeUpTime);
  void setSnooze(uint8_t snoozeMinutes);
  void handler();
  boolean alarmIsSet();
  boolean snoozeIsSet();

private:
  Radio *radio;
  RadioChannel channel = {"SWR3", "http://liveradio.swr.de/sw282p3/swr3"}; // fallback channel if not set
  struct tm wakeUpTime;
  uint8_t snoozeMinutes = DEFAULT_SNOOZE_MIN;
  boolean activated = false;
  boolean snoozed = false;
  const char *ntpServer = "pool.ntp.org";
  // get your timezone string according to this webpage: https://remotemonitoringsystems.ca/time-zone-abbreviations.php
  const char *timeZoneInfo = "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00";
  struct tm localTime;
  struct tm wakeupTime;
  struct tm snoozedTime;
};

#endif // RADIOALARM_H
