//GYRO
#include <Wire.h> //Include the Wire.h library so we can communicate with the gyro.

#define GYRO_SENSITIVITY_FACTOR 131.0

double gyro_pitch, gyro_roll, gyro_yaw;
double gyro_roll_cal, gyro_pitch_cal, gyro_yaw_cal;

int MPU = 0x68;
int gyroResult[3];
int cal_int;
