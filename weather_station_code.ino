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
  
  Serial1.println("AT+CMEE=2"); //verbal error for debuging
	delay(200);
	communicate();
  Serial1.println("AT+CFUN=1"); //activate full phone functionality
	delay(200);
	communicate();

 Serial1.println("AT+CFUN?");  //check phone functionality for debugging
	delay(200);
	communicate();

  Serial1.println("AT+COPS=0");  //choose automatic operator selection 
	delay(200);
	communicate();

  Serial1.println("AT+COPS?");  //check operator for debugging
	delay(200);
	communicate();

Serial1.println("AT+CREG=0");  //network registeration
	delay(200);
	communicate();

Serial1.println("AT+CREG?");  //check network registeration for debugging
	delay(200);
	communicate();

Serial1.println("AT+CGATT=1");  //gprs registeration 
	delay(200);
	communicate();

Serial1.println("AT+CGATT?");  //check gprs registeration for debugging
	delay(200);
	communicate();

Serial1.println("AT+CGDCONT?");  //check PDP context for debugging
	delay(200);
	communicate();

  Serial1.println("AT+CGDCONT=1,\"IP\",\"MobinilWeb\""); //setup PDP context
	delay(200);
	communicate();

Serial1.println("AT+CGACT=?");  //check PDP context activation values for debugging
	delay(200);
	communicate();

Serial1.println("AT+CGACT?");  //check if PDP context is activated for debugging
	delay(200);
	communicate();

Serial1.println("AT+CGACT=1,1");  //activate PDP context
	delay(200);
	communicate();

 Serial1.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");  //setup connection type
	delay(200);
	communicate();

  Serial1.println("AT+SAPBR=3,1,\"APN\",\"MobinilWeb\""); //setup APN
	delay(200);
	communicate();

Serial1.println("AT+SAPBR=1,1"); //use APN
	delay(200);
	communicate();

Serial1.println("AT+SAPBR=2,1"); //check APN connection for debugging
	delay(200);
	communicate();

Serial1.println("AT+HTTPINIT");  //initialise http session
	delay(200);
	communicate();

Serial1.println("AT+HTTPPARA=\"CID\",1");  //APN id 1
	delay(200);
	communicate();

Serial1.println("AT+HTTPPARA=\"URL\",\"weatherstation.wunderground.com/weatherstation/updateweatherstation.php?ID=ICAIRO9&PASSWORD=cccfshon&dateutc=now&baromin=1020&tempf=91&soilmoisture=2820&softwaretype=vws%20versionxx&action=updateraw\""); //web address to send data to
	delay(1000);
	communicate();

Serial1.println("AT+HTTPACTION=0"); //Get request
	delay(10000);
	communicate();

Serial1.println("AT+HTTPREAD"); //read data
	delay(1000);
	communicate();

Serial1.println("AT+HTTPTERM"); //terminate http session
	delay(1000);
	communicate();
 
  
}
