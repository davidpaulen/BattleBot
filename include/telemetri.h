#pragma once
#include <Arduino.h>
#include <iostream>
#include <IBusBM.h>
#include <SerialUART.h>

class Rx{
public:
    Rx(int pin_, SerialUART &serial_): pin{pin_}, serial{serial_} {}
    int getValue(int channel);                                          // Returnerar ein verdi mellom 1000 og 2000
    void setup();
    void update();
    bool isReceiving();

private:
    int pin;
    IBusBM ibus;
    SerialUART &serial;

    uint8_t lastPacketCount = 0;
    unsigned long lastPacketTime = 0;
    bool receivedPacket = false;
    unsigned long timeout = 100;
};