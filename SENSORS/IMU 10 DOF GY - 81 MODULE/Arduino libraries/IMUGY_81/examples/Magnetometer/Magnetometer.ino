#include<IMUGY_81.h>
#include<Wire.h>

IMUGY_81 IMU;
int magdata[3];

void setup() {
  // put your setup code here, to run once:
IMU.init();
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
IMU.rawMagdata(magdata);
Serial.print(magdata[0]);
Serial.print(" ");
Serial.print(magdata[1]);
Serial.print(" ");
Serial.print(magdata[2]);
Serial.println();
}
