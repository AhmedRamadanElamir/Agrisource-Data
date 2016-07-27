#include <Energia.h>
#include "BMP180.h"
#include <Wire.h>
#include "DHT11.h"
#include <OPT3001.h>
#include <Time.h>
#include "SDL_Weather_80422.h"

#define pinLED     13   // LED connected to digital pin 13
#define pinAnem    18  // Anenometer connected to pin 18 - Int 5 - Mega   / Uno pin 2
#define pinRain    2  // Anenometer connected to pin 2 - Int 0 - Mega   / Uno Pin 3 
#define intAnem    5  // int 0 (check for Uno)
#define intRain    1  // int 1
#define soil       PB_5 //PB_5 pin in Tiva C board
       
SDL_Weather_80422 weatherStation(pinAnem, pinRain, intAnem, intRain, A0, SDL_MODE_INTERNAL_AD);
TwoWire i2c_port(2);

uint8_t i;
float currentWindSpeed;
float currentWindGust;
float totalRain;
opt3001 opt3001;
char tem,hum;
int32_t temperature = 0;
int32_t pressure = 0;
double hpa_current=0;
double hpa_baseline=0;
double temperature_c=0;
int soilData;

void setup()
{
  Serial.begin(9600);
  init_BMP180();
  dht.dht_init();
  init_OPT3001();
  init_rain_wind();
  init_soil();
}

void loop()
{
  operate_BMP180();
 	 operate_DHT11();
  operate_OPT3001();
  operate_rain_wind();
  operate_soil();
  sleep(43200000);
}

void init_soil(){
	 pinMode(soil,INPUT);
}

void init_OPT3001(){
	delay(100);
  opt3001.begin(); 
}

void init_BMP180(){
  i2c_port.begin();
  delay(10);
  getCalibrationData();
  readSensor();
  hpa_baseline=pressure*0.01;
}

void init_rain_wind(){     
      weatherStation.setWindMode(SDL_MODE_SAMPLE, 5.0);
      //weatherStation.setWindMode(SDL_MODE_DELAY, 5.0);
      totalRain = 0.0;
}

void operate_soil(){ //ID:1
  soilData = analogRead(soil);
  Serial.print("1,");
  Serial.println(soilData);
  delay(100);
}

void operate_rain_wind(){ // rain ID:2, wind ID:3
  currentWindSpeed = weatherStation.current_wind_speed()/1.6;
  currentWindGust = weatherStation.get_wind_gust()/1.6;
  totalRain = totalRain + weatherStation.get_current_rain_total()/25.4;
  Serial.print("2,");
  Serial.print(totalRain);
  Serial.print("3,");
  Serial.print(currentWindSpeed);
    Serial.print(",");
  Serial.print(currentWindGust);
  Serial.print(",");
  Serial.println(weatherStation.current_wind_direction());
  delay(100);
}

void operate_BMP180(){ //ID:4
  readSensor();
  hpa_current=pressure*0.01;
  temperature_c=temperature/10.0;
  debug("4,%.2f C\r",temperature,temperature_c); 
  debug(",%.2f hPa\r\n",pressure,hpa_current);
  #define hpa_sea 1013.25
}

void operate_DHT11(){ //ID:5
  delay(200);
  dht.start_test();
  tem=dht.get_Temp();
  hum=dht.get_Hum();
  Serial.print("5,");
  Serial.println(hum,DEC);
  delay(700);
}

void operate_OPT3001(){ //ID:6
  uint32_t readings;
  readings = opt3001.readResult();  
  Serial.print("6,");
  Serial.println(readings, DEC);
  delay(800);
}


