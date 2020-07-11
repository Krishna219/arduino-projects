//GYRO
#include <Wire.h> //Include the Wire.h library so we can communicate with the gyro.

double gyro_pitch, gyro_roll, gyro_yaw;
double gyro_roll_cal, gyro_pitch_cal, gyro_yaw_cal;

int ITG = 0x68;
int gyroResult[3];
int cal_int;
