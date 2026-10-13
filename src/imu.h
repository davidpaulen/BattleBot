#pragma once
#include <Wire.h>
#include <Adafruit_BNO08x.h>

struct Point3D{
    float x;
    float y;
    float z;
};

struct Point4D{
    float w;
    float i;
    float j;
    float k;
};

class IMU{
public:
    IMU(uint8_t sda_, uint8_t scl_): sda{sda_}, scl{scl_}{};
    void setUp();
    void nullstill();
    void calibrate();
    void updateSensorValues();                  // Hentar all ny info frå IMU-en
    void updateAttitude();

    // Returnerar vinklar i radianar
    double getYaw();        
    double getPitch();
    double getRoll();

    Point3D getA() const;

    Point3D getW();

    Point4D getQ() const { return qt; }
    Point4D getQRel() const { return qRel; }
    Point4D getQ0() const { return q0; }
    
private:
    uint8_t sda = 0;
    uint8_t scl = 0;

    Adafruit_BNO08x bno08x;         // Navnet på chippen på IMU-en
    sh2_SensorValue_t sensorValue;

    Point3D a;                      // Visar akselerasjon i x, y og z i m/s²
    Point3D gyro;                   // Visar rotasjon om x, y og z og brukar høgrehandsregelen med rad/s
    Point3D magnet;                 // Visar styrken på magnetfelt i x, y og z og er målt i microtesla

    Point4D qt;                     // Visar attitude med w som gir rotasjon (w = cos(vinkel/2)) og i, j og k som gir retning på rotasjonen
    Point4D q0 = {1.0f, 0.0f, 0.0f, 0.0f};                     // Referanse 
    Point4D qRot;                   // Kva ein må gange qIMU med for å få qFly(flyet si orientering)
    Point4D qRel;                   // q relativt til q0. qRel = q0^(-1)*qt

    double Yaw;
    double Roll;
    double Pitch;

    Point4D qMultiplikasjon(Point4D q1, Point4D q2);
    Point4D qInverse(Point4D q);
};