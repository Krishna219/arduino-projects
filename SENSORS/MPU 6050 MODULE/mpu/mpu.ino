#include<Wire.h>

int MPU = 0x68;
int accResult[3], gyroResult[3];


//Function for writing a byte to an address on an I2C device
void writeTo(byte device, byte toAddress, byte val) {
  Wire.beginTransmission(device);  
  Wire.write(toAddress);        
  Wire.write(val);        
  Wire.endTransmission();
}

//Function for reading num bytes from addresses on an I2C device
void readFrom(byte device, byte fromAddress, int num, byte result[]) {
  Wire.beginTransmission(device);
  Wire.write(fromAddress);
  Wire.endTransmission();
  Wire.requestFrom((int)device, num);
  int i = 0;
  while(Wire.available()) {
    result[i] = Wire.read();
    i++;
  }
}

//Function that reads the accelerometer readings
void GetAccelerometerReadings(int Result[])
{
byte buffer[6];
readFrom(MPU, 0x3B, 6, buffer);
Result[0] = (((int)buffer[0]) << 8) | buffer[1];
Result[1] = (((int)buffer[2]) << 8) | buffer[3];
Result[2] = (((int)buffer[4]) << 8) | buffer[5];
}

//Function that reads gyroscope readings
void GetGyroscopeReadings(int Result[])
{
byte buffer[6];
readFrom(MPU, 0x43, 6, buffer);
Result[0] = (((int)buffer[0]) << 8) | buffer[1];
Result[1] = (((int)buffer[2]) << 8) | buffer[3];
Result[2] = (((int)buffer[4]) << 8) | buffer[5];
}
void setup()
{
  Wire.begin();
  writeTo(MPU, 0x6B, 0);
  Serial.begin(9600);
}
void loop()
{
GetAccelerometerReadings(accResult);
GetGyroscopeReadings(gyroResult);
Serial.print(gyroResult[0]);
Serial.print('\t');
Serial.print(gyroResult[1]);
Serial.print('\t');
Serial.print(gyroResult[2]);
Serial.print('\t');
Serial.print(accResult[0]);
Serial.print('\t');
Serial.print(accResult[1]);
Serial.print('\t');
Serial.print(accResult[2]);
Serial.println();
}
