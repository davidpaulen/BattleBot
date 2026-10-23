#include "robot.h"

void Robot::update(){
    rx.update();

    SWC = rx.getValue(6);
    SWA = rx.getValue(4);
    //if(SWA > 1250 || SWA < 900){return;}

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

void Robot::stop(){

}