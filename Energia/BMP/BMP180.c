#include "BMP180.h"
#include <Wire.h>

TwoWire i2c_port(2);

// variables to keep the values
int32_t temperature = 0;
int32_t pressure = 0;

void setup()
{
  Serial.begin(9600);

  i2c_port.begin();
  delay(10);
  getCalibrationData();
}

void loop()
{
  readSensor(temperature,pressure);
  Serial.print("temp.= ");
  Serial.print(temperature);
  Serial.print(", pressure= ");
  Serial.println(pressure);
  delay(1000);
}



