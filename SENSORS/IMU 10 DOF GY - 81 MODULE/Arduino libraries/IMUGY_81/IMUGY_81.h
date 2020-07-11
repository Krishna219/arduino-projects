/*IMUGY_81.h. This library is used to get the raw and scaled 10DOF IMU GY-81 sensor values.
The sensor contains ITG3205 Gyroscope, BMA180 accelerometer, BMP085 Pressure sensor and HMC5883L magnetometer.
Created by Krishna <satheshkishore@gmail.com> on 16/08/2015.*/

#ifndef IMUGY_81_h
#define IMUGY_81_h

#include "Arduino.h"
#include "Wire.h"


/*======ITG3205======*/
#define ITG 0x68
#define GYRO_SENSITIVITY_FACTOR 14.375

#define WHO_AM_I 0x00
#define SMPLRT_DIV 0x15
#define DLPF_FS 0x16
#define GYRO_XOUT_H 0x1D
#define GYRO_XOUT_L 0x1E
#define GYRO_YOUT_H 0x1F
#define GYRO_YOUT_L 0x20
#define GYRO_ZOUT_H 0x21
#define GYRO_ZOUT_L 0x22
#define DLPF_CFG_0 1<<0
#define DLPF_CFG_1 1<<1
#define DLPF_CFG_2 1<<2
#define DLPF_FS_SEL_0 1<<3
#define DLPF_FS_SEL_1 1<<4

#define SMPL_DIV_VALUE 9
#define FS_VALUE DLPF_FS_SEL_0|DLPF_FS_SEL_1|DLPF_CFG_0

/*======HMC5883l======*/
#define HMC 0x1E
#define RAD 0
#define DEG 1


#define MODE_REGISTER 0x02
#define X_MSB_REGISTER 0x03
#define X_LSB_REGISTER 0x04
#define Z_MSB_REGISTER 0x05
#define Z_LSB_REGISTER 0x06
#define Y_MSB_REGISTER 0x07
#define Y_LSB_REGISTER 0x08
#define CONTINUOUS_MEASUREMENT_MODE 0x00

/*======BMA180======*/
#define BMA 0x40
#define BMA180_CHIP_ID 0x00 /* chip ID - always 0x03 */
#define ACC_SENSITIVITY_FACTOR 1/8192.0 * 2 * 9.81
  
/*BMA180 Register Addresses.*/ 
#define BMA180_CHIP_VERSION 0x01 /* chip revision */ 
#define BMA180_ACC_X_LSB 0x02 /* X accel - LSB + new data */ 
#define BMA180_ACC_X_MSB 0x03 /* X accel - MSB */ 
#define BMA180_ACC_Y_LSB 0x04 /* Y accel - LSB + new data */ 
#define BMA180_ACC_Y_MSB 0x05 /* Y accel - MSB */ 
#define BMA180_ACC_Z_LSB 0x06 /* Z accel - LSB + new data */ 
#define BMA180_ACC_Z_MSB 0x07 /* Z accel - MSB */ 
#define BMA180_TEMP 0x08 /* temperature */
#define BMA180_RESET 0x10 /* soft reset */
#define BMA180_CTRL0 0x0d /* device control register 0 */
#define BMA180_BW_TCS 0x20 /* bandwidth & temp compensation BMA180 Default Setting Register Addresses*/
#define BMA180_OFFSET_LSB1 0x35 /* offset LSB X & range */

#define SOFT_RESET_VAL 0xb6 /* write value for soft reset */


//Values for  device control register 0
#define CTRL0_DIS_WAKE_UP 0x01 /* disable wake-up mode */ 
#define CTRL0_SLEEP_MODE 0x02 /* enter sleep mode */
#define CTRL0_ST0 0x04 /* self-test 0 electrostatic */ 
#define CTRL0_ST1 0x08 /* self-test 1 digital only */ 
#define CTRL0_EE_W 0x10 /* enable image/eeprom reg write */ 
#define CTRL0_UPDATE_IMAGE 0x20 /* copy eeprom to image regs */
#define CTRL0_RESET_INT 0x40 /* reset latched interrupt */
#define CTRL0_ST_DAMP 0x80 /* use damping in self test */

/* BMA180_BW_TCS  register contains the temperature compensation and bandwidth settings
 * This register contains 8-bit values 0xff i.e B11111111 
 * The first four bits contains bandwidth BW setting 0xf0 i.e B11110000
 * The last four bits contains temperature compensation TCS setting 0x0f i.e B00001111
 */
#define TCS 0x0f /* temp compensation 4 bits */
#define TCS_NEG_4_0_PCT 0x00 /* -4.0% */
#define TCS_NEG_3_5_PCT 0x01 /* -3.5% */
#define TCS_NEG_3_0_PCT 0x02 /* -3.0% */ 
#define TCS_NEG_2_5_PCT 0x03 /* -2.5% */
#define TCS_NEG_2_0_PCT 0x04 /* -2.0% */
#define TCS_NEG_1_5_PCT 0x05 /* -1.5% */
#define TCS_NEG_1_0_PCT 0x06 /* -1.0% */
#define TCS_NEG_0_5_PCT 0x07 /* -0.5% */
#define TCS_0_0_PCT 0x08 /* 0.0%  default */ 
#define TCS_0_5_PCT 0x09 /* +0.5% */ 
#define TCS_1_0_PCT 0x0a /* +1.0% */ 
#define TCS_1_5_PCT 0x0b /* +1.5% */ 
#define TCS_2_0_PCT 0x0c /* +2.0% */ 
#define TCS_2_5_PCT 0x0d /* +2.5% */ 
#define TCS_3_0_PCT 0x0e /* +3.0% */ 
#define TCS_3_5_PCT 0x0f /* +3.5% */ 
#define BW 0xf0 /* bandwidth  4 bits */ 
#define BW_10HZ 0x00 /* 10 Hz */ 
#define BW_20HZ 0x10 /* 20 Hz */ 
#define BW_40HZ 0x20 /* 40 Hz */ 
#define BW_75HZ 0x30 /* 75 Hz */ 
#define BW_150HZ 0x40 /* 150 Hz  default */ 
#define BW_300HZ 0x50 /* 300 Hz */ 
#define BW_600HZ 0x60 /* 600 Hz */ 
#define BW_1200HZ 0x70 /* 1200 Hz */ 
#define BW_HIGH_PASS 0x80 /* 1 Hz high pass filter */ 
#define BW_BAND_PASS 0x90 /* 0.2 to 300 Hz band pass fltr */

/* BMA180_OFFSET_LSB1 register contains the X-offset and range
 * This register contains 8-bit values 0xff  i.e B11111111 
 * The first four bits contains X-offset 0xf0  i.e B11110000
 * The next three bits contains RANGE setting 0x0e  i.e B00001110
 */
#define RANGE 0x0e /* accel range +/- g 3 bits */ 
#define BMA180_RANGE_1G 0x00 /* +/- 1 g */ 
#define BMA180_RANGE_1_5G 0x02 /* +/- 1.5 g */ 
#define BMA180_RANGE_2G 0x04 /* +/- 2 g */ 
#define BMA180_RANGE_3G 0x06 /* +/- 3 g */ 
#define BMA180_RANGE_4G 0x08 /* +/- 4 g */ 
#define BMA180_RANGE_8G 0x0a /* +/- 8 g */ 
#define BMA180_RANGE_16G 0x0c /* +/- 16 g */ 
#define OFFSET_X_LSB 0xf0 /* X offset low 4 bits */

class IMUGY_81
{
public:
  IMUGY_81();
  void init();
  void rawAccdata(int buff[]);
  void rawGyrodata(int buff[]);
  void rawMagdata(int buff[]);
  void getAccgravity(float buff[]);
  void getGyrodps(float buff[]);
  void getMagheading(float buff[]);
  void getMagheading(float buff[], int mode);
  void calGyro();
  void calGyro(int samples);
private:
  void writeTo(byte device, byte toAddress, byte val);
  void readFrom(byte device, byte fromAddress, int num, byte result[]);
  byte data;
  float gyro_x_offset;
  float gyro_y_offset;
  float gyro_z_offset;
  
};

#endif