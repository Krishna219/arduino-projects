#include <IMUGY_81.h>
#include <Wire.h>

#define ACC_SENSITIVITY_FACTOR 1/8192.0 * 2 * 9.81
IMUGY_81 IMU;
int accdata[3];

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
IMU.init();
}

void loop() {
  // put your main code here, to run repeatedly:
IMU.rawAccdata(accdata);
Serial.print(accdata[0] / ACC_SENSITIVITY_FACTOR);
Serial.print(" ");
Serial.print(accdata[1] / ACC_SENSITIVITY_FACTOR);
Serial.print(" ");
Serial.print(accdata[2] / ACC_SENSITIVITY_FACTOR);
Serial.println();

}
