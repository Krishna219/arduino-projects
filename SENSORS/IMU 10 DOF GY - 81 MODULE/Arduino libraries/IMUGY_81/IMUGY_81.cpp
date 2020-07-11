/*IMUGY_81.h. This library is used to get the raw and scaled 10DOF IMU GY-81 sensor values.
The sensor contains ITG3205 Gyroscope, BMA180 accelerometer, BMP085 Pressure sensor and HMC5883L magnetometer.
Created by Krishna <satheshkishore@gmail.com> on 16/08/2015.*/

#include "IMUGY_81.h"
#include "Arduino.h"
#include "Wire.h"

IMUGY_81::IMUGY_81()
{
	/*This is constructor*/
}

void IMUGY_81::init()
{
	/*This Method is to initialise all the I2C devices*/
	Wire.begin();

	/*ITG3205*/
	//Configure the gyroscope
	writeTo(ITG, DLPF_FS, FS_VALUE);/*Set the gyroscope scale for the outputs to +/-2000 degrees per second*/
	writeTo(ITG, SMPLRT_DIV, SMPL_DIV_VALUE);/*Set the sample rate to 100 hz*/

  	/*BMA180*/
  	//Configure the accelerometer
  	writeTo(BMA, BMA180_RESET, SOFT_RESET_VAL);/*Reset the accelerometer*/
  	writeTo(BMA, BMA180_CTRL0, CTRL0_EE_W);/*Enable write*/
  	readFrom(BMA, BMA180_BW_TCS, 1, &data);/*Get the current BW_TCS configuration settings*/
  	writeTo(BMA, BMA180_BW_TCS, (data & TCS)|(BW_10HZ << 4));/*Set the LPF_BW to 10Hz without changing other settings*/ 
  	readFrom(BMA, BMA180_OFFSET_LSB1, 1, &data);/*Get the current RANGE and OFFSET_X configuration settings*/
  	writeTo(BMA, BMA180_OFFSET_LSB1, (data & (OFFSET_X_LSB | 0x01))| BMA180_RANGE_2G);/*Set RANGE +/-2g without changing other settings*/

   	/*HMC5883L*/
  	//Configure the magnetometer
  	writeTo(HMC, MODE_REGISTER, CONTINUOUS_MEASUREMENT_MODE);/*Put the HMC5883 IC into the continuous operating mode*/
}


void IMUGY_81::rawAccdata(int buff[])
{
  	byte buffer[6];
  	readFrom(BMA, BMA180_ACC_X_LSB, 6, buffer);
  	buff[0] = (  (((int)buffer[1]) << 8) | buffer[0]  );
  	buff[0] = buff[0] >> 2;
  	buff[1] = (  (((int)buffer[3]) << 8) | buffer[2]  );
  	buff[1] = buff[1] >> 2;
  	buff[2] = (  (((int)buffer[5]) << 8) | buffer[4]  );
  	buff[2] = buff[2] >> 2;

}

void IMUGY_81::rawGyrodata(int buff[])
{
  	byte buffer[6];
  	readFrom(ITG, GYRO_XOUT_H, 6, buffer);
  	buff[0] = (  (((int)buffer[0]) << 8) | buffer[1]  );
  	buff[1] = (  (((int)buffer[2]) << 8) | buffer[3]  );
  	buff[2] = (  (((int)buffer[4]) << 8) | buffer[5]  );

}

void IMUGY_81::rawMagdata(int buff[])
{
  	byte buffer[6];
  	readFrom(HMC, X_MSB_REGISTER, 6, buffer);
  	buff[0] = (  (((int)buffer[0]) << 8) | buffer[1]  );
  	buff[2] = (  (((int)buffer[2]) << 8) | buffer[3]  );
  	buff[1] = (  (((int)buffer[4]) << 8) | buffer[5]  );

}

void IMUGY_81::getAccgravity(float buff[])
{
  	int accdata[3];
  	rawAccdata(accdata);
  	buff[0] = accdata[0] * ACC_SENSITIVITY_FACTOR;
  	buff[1] = accdata[1] * ACC_SENSITIVITY_FACTOR;
  	buff[2] = accdata[2] * ACC_SENSITIVITY_FACTOR;
}

void IMUGY_81::getGyrodps(float buff[])
{
  	int gyrodata[3];
  	rawGyrodata(gyrodata);
  	buff[0] = gyrodata[0] / GYRO_SENSITIVITY_FACTOR;
  	buff[1] = gyrodata[1] / GYRO_SENSITIVITY_FACTOR;
  	buff[2] = gyrodata[2] / GYRO_SENSITIVITY_FACTOR;

	buff[0] = buff[0] - gyro_x_offset;
	buff[1] = buff[1] - gyro_y_offset;
	buff[2] = buff[2] - gyro_z_offset;
	
}

void IMUGY_81::calGyro()
{
	float temp[3];
	float total[3] = {0.0, 0.0, 0.0};
	for(int i = 0; i < 10; i++)
	{
		getGyrodps(temp);
		total[0] = total[0] + temp[0];
		total[1] = total[1] + temp[1];
		total[2] = total[2] + temp[2];
	}
	gyro_x_offset = total[0] / 10.0;
	gyro_y_offset = total[1] / 10.0;
	gyro_z_offset = total[2] / 10.0;
	
}

void IMUGY_81::getMagheading(float buff[])
{
  	int magdata[3];
  	rawMagdata(magdata);
	buff[0] = atan2(magdata[1], magdata[0]);
	
}

void IMUGY_81::getMagheading(float buff[], int mode)
{
  	int magdata[3];
  	rawMagdata(magdata);
	if(mode == RAD)
  	buff[0] = atan2(magdata[1], magdata[0]);
	else if(mode == DEG)
	buff[0] = atan2(magdata[1], magdata[0]) * 180 / 3.14159;
}


void IMUGY_81::writeTo(byte device, byte toAddress, byte val)
{
	Wire.beginTransmission(device);  
	Wire.write(toAddress);        
	Wire.write(val);        
	Wire.endTransmission();
}

void IMUGY_81::readFrom(byte device, byte fromAddress, int num, byte result[])
{
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