#ifndef RADIO_H
#define RADIO_H

#include <Arduino.h>
#include <Audio.h>

typedef struct
{
  String name;
  const char *url;
} RadioChannel;

class Radio
{
public:
  Radio(Audio *audio, int ampEnable);

  boolean play(RadioChannel channel);
  void stop();
  void pause();
  void resume();
  void setVolume(uint8_t volume);

private:
  Audio *audio;

  boolean stopped = true;
  boolean paused = false;

  int ampEnable;
};

#endif // RADIO_H
