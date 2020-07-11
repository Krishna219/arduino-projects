#include<IMUGY_81.h>
#include<Wire.h>

#define GYRO_SENSITIVITY_FACTOR 14.375
IMUGY_81 IMU;
int gyrodata[3];

void setup() {
  // put your setup code here, to run once:
IMU.init();
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
IMU.rawGyrodata(gyrodata);

Serial.print(gyrodata[0] / GYRO_SENSITIVITY_FACTOR);
Serial.print(" ");
Serial.print(gyrodata[1] / GYRO_SENSITIVITY_FACTOR);
Serial.print(" ");
Serial.print(gyrodata[2] / GYRO_SENSITIVITY_FACTOR);
Serial.println();
}
