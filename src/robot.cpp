#include "robot.h"

void Robot::update(){
    rx.update();

    if(!rx.isReceiving()){
        stop();
        return;
    }

    SWC = rx.getValue(6);
    SWA = rx.getValue(4);

    // CH6 blir brukt til å oppdage radiotap
    uint16_t failsafe = rx.getValue(5);

    if(failsafe < 1750 || failsafe > 2100){
        stop();
        return;
    }

    if(SWA < 900 || SWA > 2100 || SWC < 900 || SWC > 2100){
        stop();
        return;
    }

    for(int i = 0; i < 3; i++){
        uint16_t value = rx.getValue(i);
        if(value < 900 || value > 2100){
            stop();
            return;
        }
    }

    if(rx.getValue(7) < 900 || rx.getValue(7) > 2100){
        stop();
        return;
    }

    updateSpinner();

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

    spinner.update();
}

void Robot::updateSpinner(){
    switch(spinnerState){
        case SpinnerState::WAIT_FOR_OFF:
            spinner.stopp();
            if(SWA <= 1250){
                spinnerState = SpinnerState::READY;
            }
            break;

        case SpinnerState::READY:
            if(SWA >= 1750){
                spinner.start();
                spinner.setSpeedPersent(80);
                spinnerState = SpinnerState::RUNNING;
            }
            break;

        case SpinnerState::RUNNING:
            if(SWA < 1750){
                spinner.setSpeedPersent(0);
                spinnerState = SpinnerState::READY;
            }
            break;
    }
}

void Robot::stop(){
    spinner.stopp();
    drive.set(0, 0);
    spinnerState = SpinnerState::WAIT_FOR_OFF;
}