//We’ve written an example Arduino sketch to help you get started.
//For more information visit https://www.sparkfun.com/tutorials/301 /*---HOW TO---*/

#include <Wire.h> //I2C Arduino Library

//This is a list of registers in the HMC5883L. Registers are parameters that determine how the sensor will behave, or they can hold data that represent the
//sensors current status.
//To learn more about the registers on the HMC5883L, download and read the datasheet.
//http://dlnmh9ip6v2uc.cloudfront.net/datasheets/Sensors/Magneto/HMC5883L-FDS.pdf /*---DATASHEET---*/

char MODE_REGISTER = 0x02;
char X_MSB_REGISTER = 0x03;
char X_LSB_REGISTER = 0x04;
char Z_MSB_REGISTER = 0x05;
char Z_LSB_REGISTER = 0x06;
char Y_MSB_REGISTER = 0x07;
char Y_LSB_REGISTER = 0x08;
char CONTINUOUS_MEASUREMENT_MODE = 0x00;

char hmcaddress = 0x1E; //0011110b, I2C 7bit address of HMC5883

void setup() {
  //Initialize Serial and I2C communications
  Serial.begin(9600);
  Wire.begin();

  //Put the HMC5883 IC into the correct operating mode
  Wire.beginTransmission(hmcaddress); //open communication with HMC5883
  Wire.write(MODE_REGISTER); //select mode register
  Wire.write(CONTINUOUS_MEASUREMENT_MODE); //continuous measurement mode
  Wire.endTransmission();
}

void loop() {

  int x, y, z; //triple axis data

  //Tell the HMC5883L where to begin reading data
  Wire.beginTransmission(hmcaddress);
  Wire.write(X_MSB_REGISTER ); //select register 3, X MSB register
  Wire.endTransmission();


  //Read data from each axis, 2 registers per axis
  Wire.requestFrom(hmcaddress, 6);
  if (6 <= Wire.available()) {
    x = Wire.read() << 8; //X msb
    x |= Wire.read(); //X lsb
    z = Wire.read() << 8; //Z msb
    z |= Wire.read(); //Z lsb
    y = Wire.read() << 8; //Y msb
    y |= Wire.read(); //Y lsb
  }

  //Print out values of each axis
  Serial.print("x: ");
  Serial.print(x);
  Serial.print("  y: ");
  Serial.print(y);
  Serial.print("  z: ");
  Serial.println(z);

  delay(250);
}
