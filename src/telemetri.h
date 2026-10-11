#pragma once
#include <Arduino.h>
#include <iostream>
#include <IBusBM.h>
#include <SerialUART.h>

class Rx{
public:
    Rx(int pin_, SerialUART &serial_): pin{pin_}, serial{serial_} {}
    int getValue(int channel);                                          // Returenrar ein verdi mellom 1000 og 2000
    void setup();
    void update();

private:
    int pin;
    IBusBM ibus;
    SerialUART &serial;
};