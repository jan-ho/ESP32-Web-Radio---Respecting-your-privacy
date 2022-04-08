#ifndef RADIO_H
#define RADIO_H

#include <Arduino.h>
#include <Audio.h>
#include "defines.h"

typedef struct
{
  String name;
  String url;
} RadioChannel;

class Radio
{
public:
  Radio(Audio *audio);

  boolean play(RadioChannel channel);
  void stop();
  void pause();
  void resume();
  void setVolume(uint8_t volume);
  boolean isPlaying();
  void say(const char * speech);
  void clear();

private:
  Audio *audio;

  boolean stopped = true;
  boolean paused = false;

  int ampEnable;
};

#endif // RADIO_H
