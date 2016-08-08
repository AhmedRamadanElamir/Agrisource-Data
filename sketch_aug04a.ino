void communicate(){
	 while(Serial1.available())
	 Serial.write(Serial1.read());
    }
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
  
    Serial1.println("AT+CMEE=2");
	delay(200);
	communicate();
  
}

void loop()
{
  // put your main code here, to run repeatedly:
    Serial1.println("AT+CFUN?");
	delay(200);
	communicate();

 Serial1.println("AT+CFUN=1");
	delay(200);
	communicate();

 Serial1.println("AT+COPS=0");
	delay(200);
delay(1000);
	communicate();
delay(1000);

 Serial1.println("AT+CREG?");
	delay(200);
	communicate();

delay(1000);
  
}
