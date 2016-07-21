#include <OPT3001.h>
opt3001 opt3001;
void setup()
{
  Serial.begin(9600);
  delay(1000);
  opt3001.begin(); 
}

void loop()
{
  uint32_t readings;
  readings = opt3001.readResult();  
  Serial.print("LUX Readings = ");
  Serial.println(readings, DEC);
  delay(800);
}
