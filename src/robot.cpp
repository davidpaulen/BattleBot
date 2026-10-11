#include "robot.h"

void Robot::update(){
    rx.update();

    SWC = rx.getValue(6);

    if(SWC < 1250){
        SWD = rx.getValue(7);
        imu.updateSensorValues();
        imu.updateAttitude();
        if(SWD < 1500){driveIMU.reverse = false;}
        else{driveIMU.reverse = true;}
        driveIMU.set();
    }
    else if(SWC < 1750){
        driveTank.set();
    }
    else{
        driveArcade.set();
    }
}