#pragma once
#include <Arduino.h>
#include <iostream>
#include <Servo.h>

enum class Motorstatus{
    STOPPED,
    RUNNING,
};

class BLDC_Motor{
public:
    BLDC_Motor(int pin_, int channel_): pin{pin_}, channel{channel_}{}       // Channel [0,6]. Ein kanal per motor om du ikkje vil samankople
    ~BLDC_Motor(){esc.detach();}

    void setUp();
    void start();
    void stopp();
    void setSpeedPersent(int prosent);
    void update();

private:
    int channel;
    int pin;
    int speedPersent = 0;
    int speedPersentMax = 80;
    Motorstatus status = Motorstatus::STOPPED;
    int targetSpeedPersent = 0;
    int deltaSpeedInterval = 50;
    unsigned long lastTimeUpdate = 0;
    Servo esc;

    int persentToDuty(int persent);
};

class Brushed_DC{
private:
    int RPWM;
    int LPWM;
    int REN;
    int LEN;

    float batteryVoltage;
    float motorVoltage;

    int maxPWM;

public:
    Brushed_DC(
        int rpwm,
        int lpwm,
        int ren,
        int len,
        float batteryVoltage,
        float motorVoltage = 12.0
    );

    void begin();

    // speed frå -1000 til 1000
    void setSpeed(int speed);

    void stop();

    void disable();
};