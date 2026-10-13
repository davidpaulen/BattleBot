#include "imu.h"

void IMU::setUp(){
    Wire1.setSDA(sda);
    Wire1.setSCL(scl);
    Wire1.begin();
    Wire1.setClock(100000);

    if (!bno08x.begin_I2C(0x4A, &Wire1)) {
        Serial.println("FEIL: Klarte ikkje starte BNO085!");
        return;
    }

    // Serial.println("BNO085 starta!");

    // if (!bno08x.enableReport(SH2_ACCELEROMETER))
    //     Serial.println("FEIL: Akselerometer");

    // if (!bno08x.enableReport(SH2_GYROSCOPE_CALIBRATED))
    //     Serial.println("FEIL: Gyroskop");

    // if (!bno08x.enableReport(SH2_MAGNETIC_FIELD_CALIBRATED))
    //     Serial.println("FEIL: Magnetometer");

    // if (!bno08x.enableReport(SH2_ROTATION_VECTOR))
    //     Serial.println("FEIL: Rotation Vector");

    // Serial.println("Sensorrapportar konfigurert.");
}

void IMU::updateSensorValues(){
    if(bno08x.wasReset()){
        bno08x.enableReport(SH2_ACCELEROMETER);
        bno08x.enableReport(SH2_GYROSCOPE_CALIBRATED);
        bno08x.enableReport(SH2_MAGNETIC_FIELD_CALIBRATED);
        bno08x.enableReport(SH2_ROTATION_VECTOR);
    }
    if(bno08x.getSensorEvent(&sensorValue)){
        if(sensorValue.sensorId == SH2_ACCELEROMETER){
            a = {sensorValue.un.accelerometer.x, 
                sensorValue.un.accelerometer.y, 
                sensorValue.un.accelerometer.z};
        }
        else if(sensorValue.sensorId == SH2_GYROSCOPE_CALIBRATED){
            gyro = {sensorValue.un.gyroscope.x, 
                sensorValue.un.gyroscope.y, 
                sensorValue.un.gyroscope.z};
        }
        else if(sensorValue.sensorId == SH2_MAGNETIC_FIELD_CALIBRATED){
            magnet = {sensorValue.un.magneticField.x, 
                sensorValue.un.magneticField.y, 
                sensorValue.un.magneticField.z};
        }
        else if(sensorValue.sensorId == SH2_ROTATION_VECTOR){
            qt = {sensorValue.un.rotationVector.real, 
                sensorValue.un.rotationVector.i, 
                sensorValue.un.rotationVector.j,
                sensorValue.un.rotationVector.k};
            //     //test
            //     static unsigned long lastPrint = 0;
            // if(millis() - lastPrint >= 500){
            //     lastPrint = millis();

            //     Serial.print("Quaternion W: ");
            //     Serial.println(qt.w, 4);
            // }
            //     //testslutt
        }
    }    
}

void IMU::updateAttitude(){
    qRel = qMultiplikasjon(qInverse(q0), qt);
    //     // test
    // static unsigned long lastPrint = 0;

    // if(millis() - lastPrint >= 500){
    //     lastPrint = millis();

    //     Serial.print("q0.w: ");
    //     Serial.print(q0.w, 4);

    //     Serial.print("  qRel.w: ");
    //     Serial.print(qRel.w, 4);

    //     Serial.print("  qRel.k: ");
    //     Serial.println(qRel.k, 4);
    // }
    //     // testslutt
    Yaw = atan2(2.0 * (qRel.w*qRel.k + qRel.i*qRel.j), 1.0 - 2.0 * (qRel.j*qRel.j + qRel.k*qRel.k));
    Pitch = asin(constrain(2.0 * (qRel.w*qRel.j - qRel.k*qRel.i), -1.0, 1.0));
    Roll = atan2(2.0 * (qRel.w*qRel.i + qRel.j*qRel.k), 1.0 - 2.0 * (qRel.i*qRel.i + qRel.j*qRel.j));
}

Point3D IMU::getA() const{
    return a;
}

Point3D IMU::getW(){
    return gyro;
}

// void IMU::nullstill(){
//     updateSensorValues();
//     q0 = qt;
// }

void IMU::nullstill(){
    for(int i = 0; i < 100; i++){
        updateSensorValues();

        if(qt.w != 0 || qt.i != 0 || qt.j != 0 || qt.k != 0){
            q0 = qt;
            return;
        }

        delay(5);
    }
}

double IMU::getYaw(){
    return Yaw;
}

double IMU::getRoll(){
    return Roll;
}

double IMU::getPitch(){
    return Pitch;
}

Point4D IMU::qMultiplikasjon(Point4D q1, Point4D q2){
    Point4D q;
    q.w = q1.w*q2.w - q1.i*q2.i - q1.j*q2.j - q1.k*q2.k;
    q.i = q1.w*q2.i + q1.i*q2.w + q1.j*q2.k - q1.k*q2.j;
    q.j = q1.w*q2.j - q1.i*q2.k + q1.j*q2.w + q1.k*q2.i;
    q.k = q1.w*q2.k + q1.i*q2.j - q1.j*q2.i + q1.k*q2.w;
    return q;
}

Point4D IMU::qInverse(Point4D q){
    return {q.w, -q.i, -q.j, -q.k};
}