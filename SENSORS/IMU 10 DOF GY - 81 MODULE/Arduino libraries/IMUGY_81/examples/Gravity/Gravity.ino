#include<IMUGY_81.h>
#include<Wire.h>

IMUGY_81 IMU;
float gravity[3];
void setup() {
  // put your setup code here, to run once:
IMU.init();
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
IMU.getAccgravity(gravity);
Serial.print(gravity[0]);
Serial.print(" ");
Serial.print(gravity[1]);
Serial.print(" ");
Serial.print(gravity[2]);
Serial.println();
}
