 #include <Arduino.h>
 #include "robot.h"

 BLDC_Motor spinner(22, 1);

 Brushed_DC drivL(2, 3, 4, 5, 15.3);
 Brushed_DC drivR(6, 7, 8, 9, 15.3);

 IMU imu(18, 19);
 Rx rx(17, Serial1);

 Robot Cheezslicer(spinner, drivL, drivR, rx, imu);

 void setup(){
     Serial.begin(115200);

     spinner.setUp();
     drivL.begin();
     drivR.begin();

     rx.setup();

     imu.setUp();
     imu.nullstill();

     Serial.println("Cheezslicer klar!");
 }

 void loop(){
     Cheezslicer.update();
 }
