#include "DHT11.h"

char tem,hum;
void setup()
{
dht.dht_init();
}

void loop()
{
  delay(200);
  dht.start_test();
  tem=dht.get_Temp();
  hum=dht.get_Hum();
  Serial.print("temp is =");
  Serial.println(tem,DEC);  
  Serial.print("Humid is =");
  Serial.println(hum,DEC);
  delay(700);
}

