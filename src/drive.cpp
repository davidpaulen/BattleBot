#include "drive.h"
#include <Arduino.h>

// Her er trottle -1000 til 1000
void Drive::set(int trottle, int stearing){
    int trottleL = trottle + stearing;
    int trottleR = trottle - stearing;

    trottleL = constrain(trottleL, -1000, 1000);
    trottleR = constrain(trottleR, -1000, 1000);

    motorL.setSpeed(trottleL);
    motorR.setSpeed(trottleR);
    // if(abs(trottle) > 1000){return;}
    // if(abs(stearing) > 1000){return;}

    // int trottleL = trottle + stearing;
    // int trottleR = trottle - stearing;
    // // Gjer slik at er like stor for høgare fart
    // // For over 1000
    // if(trottleL > 1000){
    //     int excess = trottleL-1000;
    //     trottleR -= excess;
    //     trottleL = 1000;
    // }
    // else if(trottleR > 1000){
    //     int excess = trottleR-1000;
    //     trottleL -= excess;
    //     trottleR = 1000;
    // }
    
    // // Under -1000
    // else if(trottleL < -1000){
    //     int excess = trottleL+1000;
    //     trottleR -= excess;
    //     trottleL = -1000;
    // }
    // else if(trottleR < -1000){
    //     int excess = trottleR + 1000;
    //     trottleL -= excess;
    //     trottleR = -1000;
    // }

    // motorL.setSpeed(trottleL);
    // motorR.setSpeed(trottleR);
}

void Drive::stop(){
    motorL.setSpeed(0);
    motorR.setSpeed(0);
}

void DriveIMU::set(){
    int ch0 = rx.getValue(0);
    int ch1 = rx.getValue(1);

    if(ch0 < 900 || ch0 > 2100 || ch1 < 900 || ch1 > 2100){
        drive.stop();
        return;
    }

    int x = ch0-1500;
    int y = ch1-1500;

    if(x*x + y*y < 30*30){
        drive.stop();
        return;
    }

    currentAngleRad = imu.getYaw();
    targetAngleRad = getTargetAngleRad(x, y);
    if(reverse){
        targetAngleRad += PI;
    }

    angleOffsetRad = targetAngleRad - currentAngleRad;
    angleOffsetRad = atan2(sin(angleOffsetRad), cos(angleOffsetRad));

    targetTrottle = getTargetTrottle(x, y);
    trottle = targetTrottle * getAngleFactor();
    if(reverse){
        trottle = -trottle;
    }
    
    // Steering
    steering = getSteering();
    drive.set(trottle, steering);


// static unsigned long lastPrint = 0;

// if(millis() - lastPrint >= 100){
//     lastPrint = millis();

//     Serial.print("Target: ");
//     Serial.print(targetAngleRad * 180.0 / PI);

//     Serial.print("  Current: ");
//     Serial.print(currentAngleRad * 180.0 / PI);

//     Serial.print("  Offset: ");
//     Serial.print(angleOffsetRad * 180.0 / PI);

//     Serial.print("  Throttle: ");
//     Serial.print(trottle);

//     Serial.print("  Steering: ");
//     Serial.println(steering);
// }

}

float DriveIMU::getTargetAngleRad(int x, int y){
    return atan2(-x, y);
}

int DriveIMU::getTargetTrottle(int x, int y){
    float theta = atan2(x,y);
    float len = sqrt((float)x*x + (float)y*y);
    float lenMax = 0;

    if((theta <= PI/4 && theta >= -PI/4) || (theta <= -3.0/4.0*PI || theta >= 3.0/4.0*PI)){
        lenMax = abs(500/cos(theta));
    }
    else{
        lenMax = abs(500/sin(theta));
    }

    return constrain(len/lenMax*1000, 0, 1000);
}

float DriveIMU::getAngleFactor(){
    return cos(angleOffsetRad);
}

int DriveIMU::getSteering(){
    float steeringError = angleOffsetRad;
    float P = maxSteering * steeringError * Kp;
    float D = imu.getW().z * Kd;
    steering = constrain((int)(P - D), -maxSteering, maxSteering);
    return steering;
}

void DriveIMU::stop(){
    drive.stop();
}


void DriveTank::set(){
    int ch2 = rx.getValue(2);
    int ch1 = rx.getValue(1);

    if(ch2 < 900 || ch2 > 2100 || ch1 < 900 || ch1 > 2100){
        stop();
        return;
    }
    
    trottleL = constrain((ch2-1500)*2, -1000, 1000);
    trottleR = constrain((ch1-1500)*2, -1000, 1000);

    if(abs(trottleL) <= 30){trottleL = 0;}
    if(abs(trottleR) <= 30){trottleR = 0;}

    motorL.setSpeed(trottleL);
    motorR.setSpeed(trottleR);
}


void DriveTank::stop(){
    motorL.setSpeed(0);
    motorR.setSpeed(0);
}


void DriveArcade::set(){
    int ch2 = rx.getValue(2);
    int ch1 = rx.getValue(1);

    if(ch2 < 900 || ch2 > 2100 || ch1 < 900 || ch1 > 2100){
        stop();
        return;
    }

    trottle = constrain((ch2 - 1500) * 2, -1000, 1000);
    steering = constrain((ch1 - 1500) * 2, -1000, 1000);

    if(abs(trottle) < 30) trottle = 0;
    if(abs(steering) < 30) steering = 0;

    drive.set(trottle, steering);
}

void DriveArcade::stop(){
    drive.stop();
}