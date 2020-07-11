#include<Wire.h>

char HMC = 0x1E;
int magResult[3];

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

//Function that reads magnetometer readings
void GetMagnetometerReadings(int Result[])
{
byte buffer[6];
readFrom(HMC, 0x03, 6, buffer);
Result[0] = (  (((int)buffer[0]) << 8) | buffer[1]  );
Result[2] = (  (((int)buffer[2]) << 8) | buffer[3]  );
Result[1] = (  (((int)buffer[4]) << 8) | buffer[5]  );
}
void setup()
{
  Wire.begin();
  writeTo(HMC, 0x02, 0x00);
  Serial.begin(9600);
}
void loop()
{
GetMagnetometerReadings(magResult);
Serial.print("X: ");
Serial.print(magResult[0]);
Serial.print(' ');
Serial.print("Y: ");
Serial.print(magResult[1]);
Serial.print(' ');
Serial.print("Z: ");
Serial.print(magResult[2]);
float heading = atan2(magResult[1], magResult[0]) * 180 / 3.14159;
Serial.print(" Heading: ");
Serial.print(heading);
Serial.println();
}
