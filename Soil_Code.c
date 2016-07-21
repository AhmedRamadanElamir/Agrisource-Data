int soil = PB_5;
int soilData;
void setup()
{
 Serial.begin(9600);
 pinMode(soil,INPUT);
}

void loop()
{
  soilData = analogRead(soil));
  Serial.println(soilData);
  delay(1000);
}
