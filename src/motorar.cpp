#include "motorar.h"

void BLDC_Motor::start(){
    if(status == Motorstatus::RUNNING){
        Serial.println("Motoren er alereide på");
        return;
    }
    status = Motorstatus::RUNNING;
    lastTimeUpdate = millis();
}

void BLDC_Motor::update(){
    if(status != Motorstatus::RUNNING || millis() - lastTimeUpdate < deltaSpeedInterval){
        return;
    }
    if(targetSpeedPersent > speedPersent){
        speedPersent += 1;
        esc.writeMicroseconds(persentToDuty(speedPersent));
        lastTimeUpdate = millis();
    }
    else if(targetSpeedPersent < speedPersent){
        speedPersent -= 1;
        esc.writeMicroseconds(persentToDuty(speedPersent));
        lastTimeUpdate = millis();
    }
}

void BLDC_Motor::stopp(){
    if(status == Motorstatus::STOPPED){
        Serial.println("Motoren er allereide av");
        return;
    }
    esc.writeMicroseconds(persentToDuty(0));
    speedPersent = 0;
    targetSpeedPersent = 0;
    status = Motorstatus::STOPPED;
}

void BLDC_Motor::setUp(){
    pinMode(pin, OUTPUT);
    esc.attach(pin, 1000, 2000);
    esc.writeMicroseconds(persentToDuty(0));
    delay(3000);
    status = Motorstatus::STOPPED;
}

int BLDC_Motor::persentToDuty(int persent){
    return 1000 + 10*persent;
}

int BLDC_Motor::dutyToPersent(int duty){
    return (duty - 1000) / 10;
}

void BLDC_Motor::setSpeedPersent(int persent){
    if(persent > 100 || persent < 0){
        Serial.println("Ugyldig verdi");
        return;
    } else if(status != Motorstatus::RUNNING){
        Serial.println("Må starte motoren");
        return;
    }
    targetSpeedPersent = persent;
}


Brushed_DC::Brushed_DC(
    int rpwm,
    int lpwm,
    int ren,
    int len,
    float batteryVoltage,
    float motorVoltage
) {
    RPWM = rpwm;
    LPWM = lpwm;
    REN = ren;
    LEN = len;

    this->batteryVoltage = batteryVoltage;
    this->motorVoltage = motorVoltage;

    // Reknar ut kor stor del av full PWM vi kan bruke
    float maxDuty = motorVoltage / batteryVoltage;

    // Kan aldri gå over 100 %
    if (maxDuty > 1.0) {
        maxDuty = 1.0;
    }

    // analogWrite brukar verdiar frå 0 til 255
    maxPWM = 255 * maxDuty;
}


void Brushed_DC::begin() {

    pinMode(RPWM, OUTPUT);
    pinMode(LPWM, OUTPUT);

    pinMode(REN, OUTPUT);
    pinMode(LEN, OUTPUT);

    // Aktiverer begge sidene av IBT-2
    digitalWrite(REN, HIGH);
    digitalWrite(LEN, HIGH);

    stop();
}


void Brushed_DC::setSpeed(int speed) {

    // speed skal vere mellom -1000 og 1000
    speed = constrain(speed, -1000, 1000);

    // Gjer speed om til PWM-verdi
    int pwm = map(
        abs(speed),
        0,
        1000,
        0,
        maxPWM
    );


    if (speed > 0) {

        // Framover
        analogWrite(LPWM, 0);
        analogWrite(RPWM, pwm);

    }
    else if (speed < 0) {

        // Bakover
        analogWrite(RPWM, 0);
        analogWrite(LPWM, pwm);

    }
    else {

        // speed = 0
        stop();
    }
}


void Brushed_DC::stop() {

    analogWrite(RPWM, 0);
    analogWrite(LPWM, 0);
}


void Brushed_DC::disable() {

    stop();

    // Slår av IBT-2
    digitalWrite(REN, LOW);
    digitalWrite(LEN, LOW);
}