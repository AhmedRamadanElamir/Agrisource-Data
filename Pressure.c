
#include <Wire.h>

TwoWire i2c_port(2);

#include <stdarg.h>
void debug(char *fmt, ... ){
  char buf[128]; // resulting string limited to 128 chars
  va_list args;
  va_start (args, fmt );
  vsnprintf(buf, 128, fmt, args);
  va_end (args);
  Serial.print(buf);
}

int I2C_ADDRESS = 0x77;  // sensor address

// oversampling setting
// 0 = ultra low power
// 1 = standard
// 2 = high
// 3 = ultra high resolution
const unsigned char oversampling_setting = 3; //oversampling for measurement
const unsigned char pressure_conversiontime[4] = { 
  5, 8, 14, 26 };  // delays for oversampling settings 0, 1, 2 and 3   
// sensor registers from the BOSCH BMP085 datasheet
int16_t ac1;
int16_t ac2; 
int16_t ac3; 
uint16_t ac4;
uint16_t ac5;
uint16_t ac6;
int16_t b1; 
int16_t b2;
int16_t mb;
int16_t mc;
int16_t md;

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

double calculate_sea_level(double p,double alt){
  return p / pow( ( 1.0 - (alt/44330.0)) , 5.255);
}


// read temperature and pressure from sensor
void readSensor() {

  int  ut= readUT();
  long up = readUdebug();

  int32_t x1, x2, x3, b3, b5, b6, p;
  uint32_t b4, b7;

  //calculate true temperature
  x1 = (ut - ac6) * ac5 >> 15;
  x2 = (mc << 11) / (x1 + md);
  b5 = x1 + x2;
  temperature = (b5 + 8) >> 4;

  //calculate true pressure
  b6 = b5 - 4000;
  x1 = (b2 * (b6 * b6 >> 12)) >> 11; 
  x2 = ac2 * b6 >> 11;
  x3 = x1 + x2;
  b3 = (((ac1 * 4 + x3)<<oversampling_setting)+2) >> 2;
  
  x1 = ac3 * b6 >> 13;
  x2 = (b1 * (b6 * b6 >> 12)) >> 16;
  x3 = ((x1 + x2) + 2) >> 2;
  b4 = (ac4 * (uint32_t) (x3 + 32768)) >> 15;
  b7 = ((uint32_t) up - b3) * (50000 >> oversampling_setting);
  if(b7 < 0x80000000){
    p= (b7 * 2) / b4;
  } 
  else {
    p= (b7 / b4) * 2;
  }

  x1 = (p >> 8) * (p >> 8);


  x1 = (x1 * 3038) >> 16;
  x2 = (-7357 * p) >> 16;
  pressure = p + ((x1 + x2 + 3791) >> 4);
}

// read uncompensated temperature value
int32_t readUT() {
  writeRegister(0xf4,0x2e);
  delay(5); // the datasheet suggests 4.5 ms
  return read_uint16_Register(0xf6);
}

// read uncompensated pressure value
int32_t readUdebug() {
  writeRegister(0xf4,0x34+(oversampling_setting<<6));
  delay(pressure_conversiontime[oversampling_setting]);

  i2c_port.beginTransmission(I2C_ADDRESS);
  i2c_port.write(0xf6);  // register to read
  i2c_port.endTransmission(false);

  i2c_port.requestFrom(I2C_ADDRESS, 3,true); // request three bytes
  while(i2c_port.available()!=3); // wait until data available
  return (  (i2c_port.read()<<16) | (i2c_port.read()<<8) | i2c_port.read() ) >>(8-oversampling_setting);
}

void  getCalibrationData() {
  ac1 = read_uint16_Register(0xAA);
  ac2 = read_uint16_Register(0xAC);
  ac3 = read_uint16_Register(0xAE);
  ac4 = read_uint16_Register(0xB0);
  ac5 = read_uint16_Register(0xB2);
  ac6 = read_uint16_Register(0xB4);
  b1 = read_uint16_Register(0xB6);
  b2 = read_uint16_Register(0xB8);
  mb = read_uint16_Register(0xBA);
  mc = read_uint16_Register(0xBC);
  md = read_uint16_Register(0xBE);
}

void writeRegister(unsigned char r, unsigned char v)
{
  i2c_port.beginTransmission(I2C_ADDRESS);
  i2c_port.write(r);
  i2c_port.write(v);
  i2c_port.endTransmission();
}

uint16_t read_uint16_Register(unsigned char r)
{
  i2c_port.beginTransmission(I2C_ADDRESS);
  i2c_port.write(r);  // register to read
  i2c_port.endTransmission(false);
  i2c_port.requestFrom(I2C_ADDRESS, 2); // request two bytes
  while(i2c_port.available()!=2); // wait until data available
  return (i2c_port.read()<<8) | i2c_port.read();
}

