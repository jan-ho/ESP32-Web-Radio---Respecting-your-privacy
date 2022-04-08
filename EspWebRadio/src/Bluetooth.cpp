#include "Bluetooth.h"

Bluetooth::Bluetooth(AutoConnect *portal)
{
    this->portal = portal;
}

void Bluetooth::start()
{
    pin_config = {
        .bck_io_num = I2S_BCLK,
        .ws_io_num = I2S_LRC,
        .data_out_num = I2S_DOUT,
        .data_in_num = I2S_PIN_NO_CHANGE};

    WiFi.disconnect();
    WiFi.mode(WIFI_MODE_NULL);
    portal->~AutoConnect(); //make as most space as possible

    bluetooth.set_pin_config(pin_config);
    digitalWrite(AMP_ENABLE, 1); // enable amp
    bluetooth.start("WebRadioBT");
    while (1)
        ;
}
