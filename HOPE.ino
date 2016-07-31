void communicate(){
	 while(Serial1.available())
	 Serial.write(Serial1.read());
    }

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
}

void loop()
{
  // put your main code here, to run repeatedly:
  Serial1.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
	delay(200);
	communicate();
	
	Serial1.println("AT+SAPBR=3,1,\"APN\",\"internet\""); //need to put your provider's APN here
	delay(200);
	communicate();

        Serial1.println("AT+CPIN?");// Tries to connect GPRS 
	delay(200);
	communicate();

	Serial1.println("AT+CREG?");// Tries to connect GPRS 
	delay(200);
	communicate();

	Serial1.println("AT+HTTPINIT");
	delay(200);
	communicate();

	Serial1.println("AT+HTTPPARA=\"CID\",1");
	delay(200);
	communicate();

	Serial1.println("AT+HTTPPARA=\"URL\",\"https://weatherstation.wunderground.com/weatherstation/updateweatherstation.php?ID=ICAIRO9&PASSWORD=cccfshon&dateutc=now&baromin=1002&tempf=31&soilmoisture=2834&softwaretype=vws%20versionxx&action=updateraw\""); //web address to send data to
	delay(200);
	communicate();

//	Serial1.println("AT+HTTPREAD=1"); //POST the data
//	communicate();

	Serial1.println("AT+HTTPTERM"); //terminate http
	delay(200);
	communicate();

	//Serial1.println("AT+SAPBR=0,1");// Disconnect GPRS
	//delay(200);
	//communicate();

  
}
