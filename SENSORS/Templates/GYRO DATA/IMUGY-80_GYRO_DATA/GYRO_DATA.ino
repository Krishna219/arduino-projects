void gyro_en()
{
  Wire.begin();

  //Use the led on the Arduino for startup indication
  digitalWrite(12,HIGH);                                       //Turn on the warning led.
  delay(3000);                                                 //Wait 2 second befor continuing.

  Wire.beginTransmission(105);                                 //Start communication with the gyro (adress 1101001)
  Wire.write(0x20);                                            //We want to write to register 1 (20 hex)
  Wire.write(0x0F);                                            //Set the register bits as 00001111 (Turn on the gyro and enable all axis)
  Wire.endTransmission();                                      //End the transmission with the gyro

  Wire.beginTransmission(105);                                 //Start communication with the gyro (adress 1101001)
  Wire.write(0x23);                                            //We want to write to register 4 (23 hex)
  Wire.write(0x90);                                            //Set the register bits as 10010000 (Block Data Update active & 500dps full scale)
  Wire.endTransmission();                                      //End the transmission with the gyro
  
  delay(250);
}

void cal_gyro()
{
  //Let's take multiple gyro data samples so we can determine the average gyro offset (calibration).
  for (cal_int = 0; cal_int < 2000 ; cal_int ++){              //Take 2000 readings for calibration.
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

void read_gyrodata(){
  Wire.beginTransmission(105);                                 //Start communication with the gyro (adress 1101001)
  Wire.write(168);                                             //Start reading @ register 28h and auto increment with every read
  Wire.endTransmission();                                      //End the transmission
  Wire.requestFrom(105, 6);                                    //Request 6 bytes from the gyro
  while(Wire.available() < 6);                                 //Wait until the 6 bytes are received
  lowByte = Wire.read();                                       //First received byte is the low part of the angular data
  highByte = Wire.read();                                      //Second received byte is the high part of the angular data
  gyro_roll = ((highByte<<8)|lowByte);                         //Multiply highByte by 256 (shift left by 8) and ad lowByte
  if(cal_int == 2000)gyro_roll -= gyro_roll_cal;               //Only compensate after the calibration
  lowByte = Wire.read();                                       //First received byte is the low part of the angular data
  highByte = Wire.read();                                      //Second received byte is the high part of the angular data
  gyro_pitch = ((highByte<<8)|lowByte);                        //Multiply highByte by 256 (shift left by 8) and ad lowByte
//  gyro_pitch *= -1;                                            //Invert axis
  if(cal_int == 2000)gyro_pitch -= gyro_pitch_cal;             //Only compensate after the calibration
  lowByte = Wire.read();                                       //First received byte is the low part of the angular data
  highByte = Wire.read();                                      //Second received byte is the high part of the angular data
  gyro_yaw = ((highByte<<8)|lowByte);                          //Multiply highByte by 256 (shift left by 8) and ad lowByte
//  gyro_yaw *= -1;                                              //Invert axis
  if(cal_int == 2000)gyro_yaw -= gyro_yaw_cal;                 //Only compensate after the calibration
}

