//The Wire library is used for I2C communication
#include <Wire.h>

//This is a list of registers in the ITG-3205. Registers are parameters that determine how the sensor will behave, or they can hold data that represent the
//sensors current status.
//To learn more about the registers on the ITG-3205, download and read the datasheet.
//http://www.thaieasyelec.com/downloads/ESEN239/%28GY-81%29ITG3205.pdf

char WHO_AM_I = 0x00;
char SMPLRT_DIV= 0x15;
char DLPF_FS = 0x16;
char GYRO_XOUT_H = 0x1D;
char GYRO_XOUT_L = 0x1E;
char GYRO_YOUT_H = 0x1F;
char GYRO_YOUT_L = 0x20;
char GYRO_ZOUT_H = 0x21;
char GYRO_ZOUT_L = 0x22;

//This is a list of settings that can be loaded into the registers.
//DLPF, Full Scale Register Bits
//FS_SEL must be set to 3 for proper operation
//Set DLPF_CFG to 3 for 1kHz Fint and 42 Hz Low Pass Filter
char DLPF_CFG_0 = 1<<0;
char DLPF_CFG_1 = 1<<1;
char DLPF_CFG_2 = 1<<2;
char DLPF_FS_SEL_0 = 1<<3;
char DLPF_FS_SEL_1 = 1<<4;

//The raw gyro x, y, z values are 16-bit data
// We have to divide it by the sensitivity rate factor to get the gyro angular rate in degrees per second
int SENSITIVITY_RATE_FACTOR = 14.375;

//I2C devices each have an address. The address is defined in the datasheet for the device. The ITG-3205 breakout board can have different address depending on how
//the jumper on top of the board is configured. By default, the jumper is connected to the VDD pin. When the jumper is connected to the VDD pin the I2C address
//is 0x68.
char itgAddress = 0x68;

//In the setup section of the sketch the serial port will be configured, the i2c communication will be initialized, and the ITG-3205 will be configured.
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

  //Configure the gyroscope
  //Set the gyroscope scale for the outputs to +/-2000 degrees per second
  itgWrite(itgAddress, DLPF_FS, (DLPF_FS_SEL_0|DLPF_FS_SEL_1|DLPF_CFG_0));
  //Set the sample rate to 100 hz
  itgWrite(itgAddress, SMPLRT_DIV, 9);
}
//The loop section of the sketch will read the X,Y and Z output rates from the gyroscope and output them in the Serial Terminal
void loop()
{
  //Create variables to hold the output rates.
  int xRate, yRate, zRate;
  //Read the x,y and z output rates from the gyroscope.
  xRate = readX() / SENSITIVITY_RATE_FACTOR;
  yRate = readY() / SENSITIVITY_RATE_FACTOR;
  zRate = readZ() / SENSITIVITY_RATE_FACTOR;
  //Print the output rates to the terminal, seperated by a TAB character.
  Serial.print(xRate);
  Serial.print('\t');
  Serial.print(yRate);
  Serial.print('\t');
  Serial.println(zRate);  

  //Wait 10ms before reading the values again. (Remember, the output rate was set to 100hz and 1reading per 10ms = 100hz.)
  delay(10);
}
//This function will write a value to a register on the ITG-3205.
//Parameters:
// char address: The I2C address of the sensor. For the ITG-3205 breakout the address is 0x68.
// char registerAddress: The address of the register on the sensor that should be written to.
// char data: The value to be written to the specified register.
void itgWrite(char address, char registerAddress, char data)
{
  //Initiate a communication sequence with the desired i2c device
  Wire.beginTransmission(address);
  //Tell the I2C address which register we are writing to
  Wire.write(registerAddress);
  //Send the value to write to the specified register
  Wire.write(data);
  //End the communication sequence
  Wire.endTransmission();
}

//This function will read the data from a specified register on the ITG-3205 and return the value.
//Parameters:
// char address: The I2C address of the sensor. For the ITG-3205 breakout the address is 0x68.
// char registerAddress: The address of the register on the sensor that should be read
//Return:
// unsigned char: The value currently residing in the specified register
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

//This function is used to read the X-Axis rate of the gyroscope. The function returns the ADC value from the Gyroscope
//NOTE: This value is NOT in degrees per second.
//Usage: int xRate = readX();
int readX(void)
{
  int data=0;
  data = itgRead(itgAddress, GYRO_XOUT_H)<<8;
  data |= itgRead(itgAddress, GYRO_XOUT_L);

  return data;
}

//This function is used to read the Y-Axis rate of the gyroscope. The function returns the ADC value from the Gyroscope
//NOTE: This value is NOT in degrees per second.
//Usage: int yRate = readY();
int readY(void)
{
  int data=0;
  data = itgRead(itgAddress, GYRO_YOUT_H)<<8;
  data |= itgRead(itgAddress, GYRO_YOUT_L);

  return data;
}

//This function is used to read the Z-Axis rate of the gyroscope. The function returns the ADC value from the Gyroscope
//NOTE: This value is NOT in degrees per second.
//Usage: int zRate = readZ();
int readZ(void)
{
  int data=0;
  data = itgRead(itgAddress, GYRO_ZOUT_H)<<8;
  data |= itgRead(itgAddress, GYRO_ZOUT_L);

  return data;
}

