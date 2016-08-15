#include <Energia.h>
#include "BMP180.h"
#include <Wire.h>
#include <Time.h>
#include <DHT11.h>
int temperature = 0;
int pressure = 0;
#define soil PB_5

void setup(){
Serial.begin(9600);
delay(1000);
Serial1.begin(9600);
pinMode(soil,INPUT);
dht.dht_init();
BMP180_init();
} 



void loop(){ 
  Serial1.println("AT+CMEE=2"); //verbal error for debuging
	delay(200);
	communicate();
Serial1.println("AT+CFUN=1"); //activate full phone functionality
	delay(200);
	communicate();
Serial1.println("AT+CFUN?");  //check phone functionality for debugging
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

  String data = "AT+HTTPPARA=\"URL\",\"agrisourcedata.com/sensors/modem.php?FID=13&SID=1&"; 
 data.concat(get_data());	
 data.concat("\"");
Serial1.println(data); //web address to send data to
	delay(1000);
communicate();

Serial1.println("AT+HTTPACTION=0"); //Get request
	delay(1000);
communicate();
Serial1.println("AT+HTTPREAD"); //read data
	delay(1000);
communicate();

Serial1.println("AT+HTTPTERM"); //terminate http session
	delay(1000);
communicate();
	delay(60000); // 1 min.
}

int  get_soil()
{   
  int soilData;
  soilData = analogRead(soil);
  return soilData;
}

String get_data(){
String data = "val1=";
readSensor(temperature, pressure);
data.concat(temperature/10.0);
data.concat("&val2=");
data.concat(pressure);
data.concat("&val3=");
dht.start_test();
int temp = dht.get_Hum();
data.concat(temp);
data.concat("&val4=");
data.concat(get_soil());
return data;
}

void communicate(){
	 while(Serial1.available())
	 Serial.write(Serial1.read()); 
}

