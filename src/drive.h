#pragma once
#include <Arduino.h>
#include <iostream>

#include "motorar.h"
#include "imu.h"
#include "telemetri.h"

class Drive{
private:
    Brushed_DC &motorL;
    Brushed_DC &motorR;
public:
    Drive(Brushed_DC &motorL_, Brushed_DC &motorR_): motorL(motorL_), motorR(motorR_){}

    void set(int trottle, int stearing);
    void stop();
};

class DriveIMU{
public:
    DriveIMU(Drive &drive_, IMU &imu_, Rx &rx_): drive(drive_), imu(imu_), rx(rx_){}
    void set();
    void stop();
    bool reverse = false;
private:
    Drive &drive;
    IMU &imu;
    Rx &rx;
    float targetAngleRad = 0;
    float currentAngleRad = 0;
    float angleOffsetRad = 0;
    int trottle = 0;
    int targetTrottle = 0;
    int steering = 0;
    const int maxSteering = 800;
    float Kp = 4.0 / PI;
    float Kd = 0;

    float getTargetAngleRad(int x, int y);
    int getTargetTrottle(int x, int y);
    float getAngleFactor();
    int getSteering();
};


class DriveTank{
public:
    DriveTank(Brushed_DC &motorL_, Brushed_DC &motorR_, Rx &rx_): motorL{motorL_}, motorR{motorR_}, rx(rx_){}
    void set();
    void stop();
private:
    Rx &rx;
    Brushed_DC &motorR;
    Brushed_DC &motorL;
    int trottleL = 0;
    int trottleR = 0;
};


class DriveArcade{
public:
    DriveArcade(Drive &drive_, Rx &rx_): drive{drive_}, rx{rx_}{}
    void set();
    void stop();
private:
    Drive &drive;
    Rx &rx;
    int trottle = 0;
    int steering = 0;
};