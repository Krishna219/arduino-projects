#include<Wire.h>

char BMA = 0x40;
int accResult[3];
byte data;
unsigned long i;

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
  while (Wire.available()) {
    result[i] = Wire.read();
    i++;
  }
}

//Function that reads magnetometer readings
void GetAccelerometerReadings(int Result[])
{
  byte buffer[6];
  readFrom(BMA, 0x02, 6, buffer);
  Result[0] = (  (((int)buffer[1]) << 8) | buffer[0]  );
  Result[0] = Result[0] >> 2;
  Result[1] = (  (((int)buffer[3]) << 8) | buffer[2]  );
  Result[1] = Result[1] >> 2;
  Result[2] = (  (((int)buffer[5]) << 8) | buffer[4]  );
  Result[2] = Result[2] >> 2;
}
void setup()
{
  Wire.begin();
  writeTo(BMA, 0x10, 0xB6);//RESET
  writeTo(BMA, 0x0D, 0x10);//ENABLE WRITE

  readFrom(BMA, 0x20, 1, &data);
  writeTo(BMA, 0x20, (data & 0x0F) | (0x00 << 4)); //SET LPF_BW to 10 Hz

  readFrom(BMA, 0x35, 1, &data);
  writeTo(BMA, 0x35, (data & 0xF1) | 0x04); //SET RANGE +/-2g

  //  Serial.begin(9600);
  DDRB |= B10000000;
  PORTB |= B10000000;
  delay(2000);
  PORTB &= B01111111;
}
void loop()
{
  GetAccelerometerReadings(accResult);

  float xg = accResult[0] / 8192.0 * 2 * 9.81;
  float yg = accResult[1] / 8192.0 * 2 * 9.81;
  float zg = accResult[2] / 8192.0 * 2 * 9.81;

  i = i + 1;
  if ( i > 10000)
    PORTB |= B10000000;
  //Serial.print("X: ");
  //Serial.print(xg);
  //Serial.print(' ');
  //Serial.print("Y: ");
  //Serial.print(yg);
  //Serial.print(' ');
  //Serial.print("Z: ");
  //Serial.print(zg);
  //Serial.println();
}
