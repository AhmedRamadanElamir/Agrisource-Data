void communicate(){
	 while(Serial1.available())
	 Serial.write(Serial1.read());
    }

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
  
    Serial1.println("AT+CMEE=2");
	delay(200);
	communicate();
  
  Serial1.println("AT+CREG=2");
	delay(200);
	communicate();

 Serial1.println("AT+CGATT=1"); 
	delay(200);
	communicate();

Serial1.println("AT+CGDCONT=1,\"IP\",\"MobinilWeb\"");
	delay(200);
	communicate();

 Serial1.println("AT+CGACT=1,1"); 
	delay(200);
	communicate();
  
   Serial1.println("AT+CGREG=2");
	delay(200);
	communicate();
  
 Serial1.println("AT+CLBS=?");
	delay(200);
	communicate();


    Serial1.println("AT+GMI");
	delay(200);
	communicate();

    Serial1.println("AT+GSN=?");
	delay(200);
	communicate();

 Serial1.println("AT+GSN");
	delay(200);
	communicate();


  Serial1.println("AT+SAPBR=?");
	delay(200);
	communicate();

Serial1.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
	delay(200);
	communicate();

	Serial1.println("AT+SAPBR=3,1,\"APN\",\"MobinilWeb\""); //need to put your provider's APN here
	delay(200);
	communicate();

Serial1.println("AT+SAPBR=3,1,\"USER\",\"\""); //need to put your provider's APN here
	delay(200);
	communicate();

Serial1.println("AT+SAPBR=3,1,\"PWD\",\"\""); //need to put your provider's APN here
	delay(200);
	communicate();

Serial1.println("AT+SAPBR=3,1,\"PHONENUM\",\"01200910420\""); //need to put your provider's APN here
	delay(200);
	communicate();


Serial1.println("AT+SAPBR=1,1");
	delay(200);
	communicate();

delay(5000);

Serial1.println("AT+SAPBR=2,1");
	delay(200);
	communicate();

}

void loop()
{
  
  
   Serial1.println("AT+CREG?"); 
	delay(200);
	communicate();

Serial1.println("AT+CGATT?"); 
	delay(200);
	communicate();
  
  
  // put your main code here, to run repeatedly:
     Serial1.println("AT+CSQ=?"); 
	delay(200);
	communicate();

 Serial1.println("AT+CSQ"); 
	delay(200);
	communicate();



      Serial1.println("AT+SAPBR=2,1"); 
	delay(200);
	communicate();


	Serial1.println("AT+HTTPINIT=?");
	delay(200);
	communicate();

	Serial1.println("AT+HTTPINIT");
	delay(200);
	communicate();

Serial1.println("AT+HTTPPARA=?");
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

	
Serial1.println("AT+HTTPACTION=?");
	delay(200);
	communicate();

Serial1.println("AT+HTTPACTION=0");
	delay(200);
	communicate();

Serial1.println("AT+HTTPREAD=?");
	delay(200);
	communicate();

Serial1.println("AT+HTTPTERM=?");
	delay(200);
	communicate();

Serial1.println("AT+HTTPTERM");
	delay(200);
	communicate();

	
  
}
