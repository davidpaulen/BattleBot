#include <Arduino.h>

#include "motorar.h"
#include "drive.h"
#include "telemetri.h"
#include "imu.h"

// ======================================================
// PINNAR
// ======================================================

// Venstre IBT-2
constexpr uint8_t L_RPWM = 2;
constexpr uint8_t L_LPWM = 3;
constexpr uint8_t L_REN  = 4;
constexpr uint8_t L_LEN  = 5;

// Høgre IBT-2
constexpr uint8_t R_RPWM = 6;
constexpr uint8_t R_LPWM = 7;
constexpr uint8_t R_REN  = 8;
constexpr uint8_t R_LEN  = 9;

// FlySky iBUS
constexpr uint8_t RX_PIN = 13;

// BNO085
constexpr uint8_t IMU_SDA = 20;
constexpr uint8_t IMU_SCL = 21;

// ======================================================
// OBJEKT
// ======================================================

Brushed_DC motorL(
    L_RPWM,
    L_LPWM,
    L_REN,
    L_LEN,
    9.2
);

Brushed_DC motorR(
    R_RPWM,
    R_LPWM,
    R_REN,
    R_LEN,
    9.2
);

Drive drive(motorL, motorR);

Rx receiver(RX_PIN, Serial1);

IMU imu(IMU_SDA, IMU_SCL);

DriveIMU driveIMU(drive, imu, receiver);

// ======================================================
// SETUP
// ======================================================

void setup()
{
    Serial.begin(115200);

    delay(1500);

    Serial.println("=== CHEESSLICER IMU TEST ===");

    // Motorar
    motorL.begin();
    motorR.begin();

    drive.stop();

    // RX
    Serial1.setRX(RX_PIN);
    receiver.setup();

    // IMU
    imu.setUp();

    delay(100);
    imu.nullstill();

    Serial.println("System klart!");
}

// ======================================================
// LOOP
// ======================================================

void loop()
{
    // Oppdater RX
    receiver.update();

    // Oppdater IMU
    imu.updateSensorValues();
    imu.updateAttitude();

    // Les kanalane
    uint16_t CH0 = receiver.getValue(0);
    uint16_t CH1 = receiver.getValue(1);

    // Failsafe
    if (CH0 < 900 || CH0 > 2100 ||
        CH1 < 900 || CH1 > 2100)
    {
        driveIMU.stop();
        return;
    }

    // IMU-styring
    driveIMU.set();

    // Debug
    static uint32_t lastPrint = 0;

    if (millis() - lastPrint >= 100)
    {
        Serial.print("CH0: ");
        Serial.print(CH0);

        Serial.print(" CH1: ");
        Serial.print(CH1);

        Serial.print(" Yaw: ");
        Serial.print(imu.getYaw() * 180.0 / PI);

        Serial.print(" Pitch: ");
        Serial.print(imu.getPitch() * 180.0 / PI);

        Serial.print(" Roll: ");
        Serial.println(imu.getRoll() * 180.0 / PI);

        lastPrint = millis();
    }
}