#include<Wire.h>

char ITG = 0x68;
int gyroResult[3];

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

//Function that reads gyroscope readings
void GetGyroscopeReadings(int Result[])
{
byte buffer[6];
readFrom(ITG, 0x1D, 6, buffer);
Result[0] = (  (((int)buffer[0]) << 8) | buffer[1]  );
Result[1] = (  (((int)buffer[2]) << 8) | buffer[3]  );
Result[2] = (  (((int)buffer[4]) << 8) | buffer[5]  );
}
void setup()
{
  Wire.begin();
  writeTo(ITG, 0x16, 0x19);
  writeTo(ITG, 0x15, 0x9);
  Serial.begin(9600);
}
void loop()
{
GetGyroscopeReadings(gyroResult);
Serial.print(gyroResult[0] / 14.375);
Serial.print('\t');
Serial.print(gyroResult[1] / 14.375);
Serial.print('\t');
Serial.print(gyroResult[2] / 14.375);
Serial.println();
}
