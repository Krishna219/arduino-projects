//All register and initialisation informations are obtained from these links
//http://asf.atmel.com/docs/latest/xmegad/html/bma180_8h.html#a77300c35bab254e483e72cef5ab79c44
//http://asf.atmel.com/docs/latest/xmegad/html/bma180_8c.html#a401a33ce53e2d68576e772ae89875841

//ALl calculations and convertion factors are refered from following sensor libraries
//ADXL345
//https://github.com/adafruit/Adafruit_ADXL345
//BMA180
//https://www.dropbox.com/sh/ttqiw9binmte3n5/AABBfRenBbKmO-bnQwEvIg5za?dl=0


#define DEBUG

#include<Wire.h>

#define BMA180_ADDRESS 0x40
#define BMA180_CHIP_ID   (0x00) /* chip ID - always 0x03 */
  
/*BMA180 Register Addresses.*/ 
#define   BMA180_CHIP_VERSION   (0x01) /* chip revision */ 
#define   BMA180_ACC_X_LSB   (0x02) /* X accel - LSB + new data */ 
#define   BMA180_ACC_X_MSB   (0x03) /* X accel - MSB */ 
#define   BMA180_ACC_Y_LSB   (0x04) /* Y accel - LSB + new data */ 
#define   BMA180_ACC_Y_MSB   (0x05) /* Y accel - MSB */ 
#define   BMA180_ACC_Z_LSB   (0x06) /* Z accel - LSB + new data */ 
#define   BMA180_ACC_Z_MSB   (0x07) /* Z accel - MSB */ 
#define   BMA180_TEMP   (0x08) /* temperature */
#define   BMA180_RESET   (0x10) /* soft reset */
#define   BMA180_CTRL0   (0x0d) /* device control register 0 */
#define   BMA180_BW_TCS   (0x20) /* bandwidth & temp compensation BMA180 Default Setting Register Addresses*/
#define   BMA180_OFFSET_LSB1   (0x35) /* offset LSB (X) & range */

#define SOFT_RESET_VAL   (0xb6) /* write value for soft reset */


//Values for  device control register 0
#define   CTRL0_DIS_WAKE_UP   (0x01) /* disable wake-up mode */ 
#define   CTRL0_SLEEP_MODE   (0x02) /* enter sleep mode */
#define   CTRL0_ST0   (0x04) /* self-test 0 (electrostatic) */ 
#define   CTRL0_ST1   (0x08) /* self-test 1 (digital only) */ 
#define   CTRL0_EE_W   (0x10) /* enable image/eeprom reg write */ 
#define   CTRL0_UPDATE_IMAGE   (0x20) /* copy eeprom to image regs */
#define   CTRL0_RESET_INT   (0x40) /* reset latched interrupt */
#define   CTRL0_ST_DAMP   (0x80) /* use damping in self test */
#define   CTRL0_EE_W   (0x10) /* enable image/eeprom reg write */

/* BMA180_BW_TCS  register contains the temperature compensation and bandwidth settings
 * This register contains 8-bit values 0xff (i.e) B11111111 
 * The first four bits contains bandwidth BW setting 0xf0 (i.e) B11110000
 * The last four bits contains temperature compensation TCS setting 0x0f (i.e) B00001111
 */
#define   TCS   (0x0f) /* temp compensation (4 bits) */
#define   TCS_NEG_4_0_PCT   (0x00) /* -4.0% */
#define   TCS_NEG_3_5_PCT   (0x01) /* -3.5% */
#define   TCS_NEG_3_0_PCT   (0x02) /* -3.0% */ 
#define   TCS_NEG_2_5_PCT   (0x03) /* -2.5% */
#define   TCS_NEG_2_0_PCT   (0x04) /* -2.0% */
#define   TCS_NEG_1_5_PCT   (0x05) /* -1.5% */
#define   TCS_NEG_1_0_PCT   (0x06) /* -1.0% */
#define   TCS_NEG_0_5_PCT   (0x07) /* -0.5% */
#define   TCS_0_0_PCT   (0x08) /* 0.0% (default) */ 
#define   TCS_0_5_PCT   (0x09) /* +0.5% */ 
#define   TCS_1_0_PCT   (0x0a) /* +1.0% */ 
#define   TCS_1_5_PCT   (0x0b) /* +1.5% */ 
#define   TCS_2_0_PCT   (0x0c) /* +2.0% */ 
#define   TCS_2_5_PCT   (0x0d) /* +2.5% */ 
#define   TCS_3_0_PCT   (0x0e) /* +3.0% */ 
#define   TCS_3_5_PCT   (0x0f) /* +3.5% */ 
#define   BW   (0xf0) /* bandwidth (4 bits) */ 
#define   BW_10HZ   (0x00) /* 10 Hz */ 
#define   BW_20HZ   (0x10) /* 20 Hz */ 
#define   BW_40HZ   (0x20) /* 40 Hz */ 
#define   BW_75HZ   (0x30) /* 75 Hz */ 
#define   BW_150HZ   (0x40) /* 150 Hz (default) */ 
#define   BW_300HZ   (0x50) /* 300 Hz */ 
#define   BW_600HZ   (0x60) /* 600 Hz */ 
#define   BW_1200HZ   (0x70) /* 1200 Hz */ 
#define   BW_HIGH_PASS   (0x80) /* 1 Hz high pass filter */ 
#define   BW_BAND_PASS   (0x90) /* 0.2 to 300 Hz band pass fltr */

/* BMA180_OFFSET_LSB1 register contains the X-offset and range
 * This register contains 8-bit values 0xff (i.e) B11111111 
 * The first four bits contains X-offset 0xf0 (i.e) B11110000
 * The next three bits contains RANGE setting 0x0e (i.e) B00001110
 */
#define   RANGE   (0x0e) /* accel range +/- g (3 bits) */ 
#define   BMA180_RANGE_1G   (0x00) /* +/- 1 g */ 
#define   BMA180_RANGE_1_5G   (0x02) /* +/- 1.5 g */ 
#define   BMA180_RANGE_2G   (0x04) /* +/- 2 g */ 
#define   BMA180_RANGE_3G   (0x06) /* +/- 3 g */ 
#define   BMA180_RANGE_4G   (0x08) /* +/- 4 g */ 
#define   BMA180_RANGE_8G   (0x0a) /* +/- 8 g */ 
#define   BMA180_RANGE_16G   (0x0c) /* +/- 16 g */ 
#define   OFFSET_X_LSB   (0xf0) /* X offset (low 4 bits) */

byte data;
int accx, accy, accz;
unsigned long i;


void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Wire.begin();

//Reset the accelerometer
Wire.beginTransmission(BMA180_ADDRESS);   // address of the accelerometer
Wire.write(BMA180_RESET); 
Wire.write(SOFT_RESET_VAL);
Wire.endTransmission();


//REnable write
Wire.beginTransmission(BMA180_ADDRESS);   // address of the accelerometer
Wire.write(BMA180_CTRL0); 
Wire.write(CTRL0_EE_W);
Wire.endTransmission();


//set low pass filter to 10hz in BMA180_BW_TCS  register
Wire.beginTransmission(BMA180_ADDRESS);    // address of the accelerometer
Wire.write(BMA180_BW_TCS );               // read from here
Wire.endTransmission();
Wire.requestFrom(BMA180_ADDRESS, 1);
data = Wire.read();
Wire.beginTransmission(BMA180_ADDRESS);   // address of the accelerometer
Wire.write(BMA180_BW_TCS );
Wire.write((data & TCS) | BW_10HZ<<4);       // low pass filter to 10 Hz
Wire.endTransmission();
delay(10);

//set accelerometer sensitivity (i.e) RANGE to +/- 2g
Wire.beginTransmission(BMA180_ADDRESS);   // address of the accelerometer
Wire.write(BMA180_OFFSET_LSB1);               // read from here
Wire.endTransmission();
Wire.requestFrom(BMA180_ADDRESS, 1);
data = Wire.read();
Wire.beginTransmission(BMA180_ADDRESS);   // address of the accelerometer
Wire.write(BMA180_OFFSET_LSB1);
Wire.write((data & (OFFSET_X_LSB | 0x01)) | BMA180_RANGE_2G);// range +/- 2g
Wire.endTransmission();

  
//Getting device id
Wire.beginTransmission(BMA180_ADDRESS);   // address of the accelerometer
Wire.write(0x00);               // set read pointer to data 
Wire.endTransmission();
Wire.requestFrom(BMA180_ADDRESS, 2);
while(Wire.available() < 2);
char id = Wire.read();
char v= Wire.read();
Serial.print("ID : ");
Serial.print(id, HEX);
Serial.print(" v ");
Serial.println(v, HEX);

DDRB |= B10000000;
PORTB |= B10000000;
delay(2000);
PORTB &= B01111111;
//Yeah we made it.
//Now its time to read accelerometer data

}

void loop() {
  // put your main code here, to run repeatedly:
Wire.beginTransmission(BMA180_ADDRESS);   // address of the accelerometer
Wire.write(BMA180_ACC_X_LSB);               // set read pointer to data
Wire.endTransmission();
Wire.requestFrom(BMA180_ADDRESS,6);
while(Wire.available() < 6);
accx = Wire.read();
accx |= Wire.read() << 8;
accx = accx >> 2;
accy = Wire.read();
accy |= Wire.read() << 8;
accy = accy >> 2;
accz = Wire.read();
accz |= Wire.read() << 8;
accz = accz >> 2;

float xg = accx/8192.0 * 2 * 9.81;
float yg = accy/8192.0 * 2 * 9.81;
float zg = accz/8192.0 * 2 * 9.81;

Serial.print("X: ");
Serial.print(xg, 4);
Serial.print(" Y: ");
Serial.print(yg, 4);
Serial.print(" Z: ");
Serial.println(zg, 4);


////Print mode
//Wire.beginTransmission(BMA180_ADDRESS);   // address of the accelerometer
//Wire.write(BMA180_OFFSET_LSB1);               // read from here
//Wire.endTransmission();
//Wire.requestFrom(BMA180_ADDRESS, 1);
//data = Wire.read();
//Serial.print(" MODE: ");
//Serial.print(data, BIN);
}
