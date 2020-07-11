#include<IMUGY_81.h>
#include<Wire.h>

IMUGY_81 IMU;
float dps[3];

void format_print(float val)
{
  if(val < 0) Serial.print('-');
  else Serial.print('+');
  if(abs(val) / 100 < 1) Serial.print('0');
  if(abs(val) / 10 < 1) Serial.print('0');
  Serial.print(abs(val));
  delay(10);
}
void setup() {
  // put your setup code here, to run once:
IMU.init();
IMU.calGyro();
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
IMU.getGyrodps(dps);
format_print(dps[0]);
Serial.print(" ");
format_print(dps[1]);
Serial.print(" ");
format_print(dps[2]);
Serial.println();
}
