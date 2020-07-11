//GYRO
#include <Wire.h> //Include the Wire.h library so we can communicate with the gyro.

#define GYRO_SENSITIVITY_FACTOR 57.14286

double gyro_pitch, gyro_roll, gyro_yaw;
double gyro_roll_cal, gyro_pitch_cal, gyro_yaw_cal;
byte highByte, lowByte;
int cal_int;
