#include "Radio.h"

Radio::Radio(Audio *audio, int ampEnable)
{
  this->audio = audio;
  this->ampEnable = ampEnable;
  pinMode(ampEnable, OUTPUT);
  digitalWrite(ampEnable, 0); // shutdown amp for powersaving
}

boolean Radio::play(RadioChannel channel)
{
  paused = false;
  stopped = false;
  digitalWrite(ampEnable, 1); // enable amp

  if (!audio->connecttohost(channel.url))
  {
    Serial.printf("Failed to connect to %s\n", channel.url);
    return false;
  }
  return true;
}

void Radio::stop()
{
  if (stopped)
  {
    return;
  }
  stopped = true;
  audio->stopSong();
  digitalWrite(ampEnable, 0); // shutdown amp for powersaving
}

void Radio::pause()
{
  if (paused)
  {
    return;
  }
  paused = true;
  audio->pauseResume();
  digitalWrite(ampEnable, 0); // shutdown amp for powersaving
}

void Radio::resume()
{
  if (!paused)
  {
    return;
  }
  digitalWrite(ampEnable, 1); // enable amp
  paused = false;
  audio->pauseResume();
}

void Radio::setVolume(uint8_t volume)
{
  volume = (volume < 0) ? 0 : (volume > 100) ? 100
                                             : volume;
  audio->setVolume(volume * 21 / 100);
}
