#pragma once
#include <Arduino.h>
#include <iostream>
#include <motorar.h>
#include <telemetri.h>
#include <drive.h>

class Robot{
public:
    Robot(BLDC_Motor &spinner_, 
        Brushed_DC &drivMotorL_, 
        Brushed_DC &drivMotorR_, 
        Rx &rx_,
        IMU &imu_):

        spinner{spinner_}, 
        drivMotorL{drivMotorL_}, 
        drivMotorR{drivMotorR_},
        rx{rx_},
        imu{imu_},
        drive{drivMotorL_, drivMotorR_},
        driveIMU{drive, imu_, rx_},
        driveTank{drivMotorL_, drivMotorR_, rx_},
        driveArcade{drive, rx_}{}
    void update();
private:
    BLDC_Motor &spinner;
    Brushed_DC &drivMotorL;
    Brushed_DC &drivMotorR;
    Rx &rx;
    IMU &imu;
    Drive drive;
    DriveIMU driveIMU;
    DriveTank driveTank;
    DriveArcade driveArcade;
    uint16_t SWC = 0;
    uint16_t SWD = 0;
};