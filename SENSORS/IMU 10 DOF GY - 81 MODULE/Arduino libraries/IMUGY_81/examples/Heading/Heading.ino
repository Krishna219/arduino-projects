#include<IMUGY_81.h>
#include<Wire.h>

IMUGY_81 IMU;
float heading;

void setup() {
  // put your setup code here, to run once:
IMU.init();
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
IMU.getMagheading(&heading, DEG);
Serial.println(heading);
}
