#include <Wire.h>


char itgAddress = 0x68;

unsigned char itgRead(char address, char registerAddress)
{
  //This variable will hold the contents read from the i2c device.
  unsigned char data=0;

  //Send the register address to be read.
  Wire.beginTransmission(address);
  //Send the Register Address
  Wire.write(registerAddress);
  //End the communication sequence.
  Wire.endTransmission();

  //Ask the I2C device for data
  Wire.beginTransmission(address);
  Wire.requestFrom(address, 1);

  //Wait for a response from the I2C device
  if(Wire.available()){
    //Save the data sent from the I2C device
    data = Wire.read();
  }

  //End the communication sequence.
  Wire.endTransmission();

  //Return the data read during the operation
  return data;
}

void setup()
{
  //Create a serial connection using a 9600bps baud rate.
  Serial.begin(9600);

  //Initialize the I2C communication. This will set the Arduino up as the 'Master' device.
  Wire.begin();

  //Read the WHO_AM_I register and print the result
  char id=0; 
  id = itgRead(itgAddress, 0x00);  
  Serial.print("ID: ");
  Serial.println(id, HEX);
}

void loop()
{
}

