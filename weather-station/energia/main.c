#include <Energia.h>
#include "BMP180.h"
#include <Wire.h>
#include <OPT3001.h>
#include <Time.h>
#include <flow.h>
#include <DHT11.h>


void setup(){
Serial.begin(9600);
Serial1.begin(9600);
} 
void loop(){

	Serial1.println("AT+CSCLK=0");
  	delay(200);
  Serial1.println("AT+CFUN=1"); //activate full phone functionality
	delay(200);
  Serial1.println("AT+COPS=0");  //choose automatic operator selection 
	delay(200);
Serial1.println("AT+CREG=0");  //network registeration
	delay(200);
Serial1.println("AT+CGATT=1");  //gprs registeration 
	delay(200);
  Serial1.println("AT+CGDCONT=1,\"IP\",\"MobinilWeb\""); //setup PDP context
	delay(200);
Serial1.println("AT+CGACT=1,1");  //activate PDP context
	delay(200);
 Serial1.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");  //setup connection type
	delay(200);
  Serial1.println("AT+SAPBR=3,1,\"APN\",\"MobinilWeb\""); //setup APN
	delay(200);
Serial1.println("AT+SAPBR=1,1"); //use APN
	delay(200);
Serial1.println("AT+HTTPINIT");  //initialise http session
	delay(200);
Serial1.println("AT+HTTPPARA=\"CID\",1");  //APN id 1
	delay(200);

// String data = "agrisourcedata.com/sensors/modem.php?FID=13&SID=1&";  read the data here	
Serial1.println("AT+HTTPPARA=\"URL\",\"agrisourcedata.com/sensors/modem.php?FID=13&SID=1&val1=2&val2=2&val3=2&val4=2\""); //web address to send data to
	delay(1000);
Serial1.println("AT+HTTPACTION=0"); //Get request
	delay(10000);
Serial1.println("AT+HTTPREAD"); //read data
	delay(1000);


Serial1.println("AT+HTTPTERM"); //terminate http session
	delay(1000);
	communicate();

	delay(1000);
	communicate(); 
 
 	Serial1.println("AT+CSCLK=1");
  	delay(200);
}
}
