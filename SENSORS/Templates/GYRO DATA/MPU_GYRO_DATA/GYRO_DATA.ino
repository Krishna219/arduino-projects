void gyro_en()
{
  Wire.begin();

  //Use the led on the Arduino for startup indication
  digitalWrite(12, HIGH);                                      //Turn on the warning led.
  delay(3000);                                                 //Wait 2 second befor continuing.

  writeTo(MPU, 0x6B, 0);
  writeTo(MPU, 0x1B, 0);

  delay(250);
}

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

void read_gyrodata() {
  byte buffer[6];
  readFrom(MPU, 0x43, 6, buffer);
  gyroResult[0] = (((int)buffer[0]) << 8) | buffer[1];
  gyroResult[1] = (((int)buffer[2]) << 8) | buffer[3];
  gyroResult[2] = (((int)buffer[4]) << 8) | buffer[5];

  gyro_roll = gyroResult[0];
  if (cal_int == 2000)gyro_roll -= gyro_roll_cal;              //Only compensate after the calibration

  gyro_pitch = gyroResult[1];
  //  gyro_pitch *= -1;                                            //Invert axis
  if (cal_int == 2000)gyro_pitch -= gyro_pitch_cal;            //Only compensate after the calibration

  gyro_yaw = gyroResult[2];
  //  gyro_yaw *= -1;                                              //Invert axis
  if (cal_int == 2000)gyro_yaw -= gyro_yaw_cal;                //Only compensate after the calibration
}

void cal_gyro()
{
  //Let's take multiple gyro data samples so we can determine the average gyro offset (calibration).
  for (cal_int = 0; cal_int < 2000 ; cal_int ++) {             //Take 2000 readings for calibration.
    //    if(cal_int % 15 == 0)digitalWrite(12, !digitalRead(12));   //Change the led status to indicate calibration.
    read_gyrodata();                                          //Read the gyro output.
    gyro_roll_cal += gyro_roll;                                //Ad roll value to gyro_roll_cal.
    gyro_pitch_cal += gyro_pitch;                              //Ad pitch value to gyro_pitch_cal.
    gyro_yaw_cal += gyro_yaw;                                  //Ad yaw value to gyro_yaw_cal.

  }
  //Now that we have 2000 measures, we need to devide by 2000 to get the average gyro offset.
  gyro_roll_cal /= 2000;                                       //Divide the roll total by 2000.
  gyro_pitch_cal /= 2000;                                      //Divide the pitch total by 2000.
  gyro_yaw_cal /= 2000;
}

