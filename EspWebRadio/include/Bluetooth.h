#ifndef BLUETOOTH_H
#define BLUETOOTH_H



#include "BluetoothA2DPSink.h"
#include "AutoConnect.h"
#include "defines.h"


class Bluetooth
{
public:
  Bluetooth(AutoConnect *portal);
  void start();

private:
BluetoothA2DPSink bluetooth;
AutoConnect *portal;

i2s_pin_config_t pin_config;
  
};

#endif // BLUETOOTH_H
