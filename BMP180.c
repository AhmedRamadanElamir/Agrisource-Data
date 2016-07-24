#include "BMP180.h"
#include <Wire.h>

TwoWire i2c_port(2);

// variables to keep the values
int32_t temperature = 0;
int32_t pressure = 0;

double hpa_current=0;
double hpa_baseline=0;
double temperature_c=0;

void setup()
{
  Serial.begin(9600);

  i2c_port.begin();
  delay(10);
  getCalibrationData();
  
  readSensor();
  hpa_baseline=pressure*0.01;
}

void loop()
{
  readSensor();
  hpa_current=pressure*0.01;
  temperature_c=temperature/10.0;
  
  debug("Temperature:%.2f C\r\n",temperature,temperature_c); 
  debug("Pressure:%.2f hPa\r\n",pressure,hpa_current);

#define hpa_sea 1013.25
  delay(1000);
}



