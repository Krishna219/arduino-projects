void setup()
{
  Serial.begin(57600);
  gyro_en();
  cal_gyro();
}

void loop()
{
  read_gyrodata();
  print_gyrodata();
  Serial.println();
}

