void communicate(){
	 while(Serial1.available())
	 Serial.write(Serial1.read());
 
}

void loop()
{
  // put your main code here, to run repeatedly:
  

  
    }
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial);
   
  //Being serial communication witj Arduino and SIM800
  Serial1.begin(9600);
  delay(1000);
  
  Serial1.println("AT+CMEE=2");
	delay(200);
	communicate();
  Serial1.println("AT+CFUN=1");
	delay(200);
	communicate();

 Serial1.println("AT+CFUN?");
	delay(200);
	communicate();

  Serial1.println("AT+COPS=0");
	delay(200);
	communicate();

  Serial1.println("AT+COPS?");
	delay(200);
	communicate();

Serial1.println("AT+CREG=0");
	delay(200);
	communicate();

Serial1.println("AT+CREG?");
	delay(200);
	communicate();

Serial1.println("AT+CGATT=1");
	delay(200);
	communicate();

Serial1.println("AT+CGATT?");
	delay(200);
	communicate();

Serial1.println("AT+CGDCONT?");
	delay(200);
	communicate();

  Serial1.println("AT+CGDCONT=1,\"IP\",\"MobinilWeb\"");
	delay(200);
	communicate();

Serial1.println("AT+CGACT=1,1");
	delay(200);
	communicate();

 Serial1.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
	delay(200);
	communicate();

  Serial1.println("AT+SAPBR=3,1,\"APN\",\"MobinilWeb\""); //need to put your provider's APN here
	delay(200);
	communicate();

Serial1.println("AT+SAPBR=1,1"); //need to put your provider's APN here
	delay(200);
	communicate();

Serial1.println("AT+SAPBR=2,1"); //need to put your provider's APN here
	delay(200);
	communicate();

Serial1.println("AT+HTTPINIT");
	delay(200);
	communicate();

Serial1.println("AT+HTTPPARA=\"CID\",1");
	delay(200);
	communicate();

Serial1.println("AT+HTTPPARA=\"URL\",\"https://weatherstation.wunderground.com/weatherstation/updateweatherstation.php?ID=ICAIRO9&PASSWORD=cccfshon&dateutc=now&baromin=1020&tempf=91&soilmoisture=2820&softwaretype=vws%20versionxx&action=updateraw\""); //web address to send data to
	delay(200);
	communicate();

Serial1.println("AT+HTTPACTION=0"); //web address to send data to
	delay(200);
	communicate();

Serial1.println("AT+HTTPREAD"); //web address to send data to
	delay(200);
	communicate();
 
  
}
