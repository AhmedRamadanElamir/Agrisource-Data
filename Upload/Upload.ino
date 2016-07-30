#include <SIM800l.h>

String id = "ICAIRO9";
String PASSWORD = "cccfshon";
String URL = "https://weatherstation.wunderground.com/weatherstation/updateweatherstation.php?";

SIM800l SIM800l;
HardwareSerial Serial1;
void setup(){
Serial.begin(9600);
Serial1.begin(9600);
SIM800l(Serial1);
SIM800l.init(9600);

} 
void loop(){
String data = URL;
data.concat("ID=");
data.concat(id);
data.concat("&");
data.concat("PASSWORD=");
data.concat(PASSWORD);
data.concat("&dateutc=now&");
data = get_BMP180();	
data.concat("&");
data.concat(get_DHT11());
data.concat("&");
data.concat(get_OPT3001());
data.concat("&");
data.concat(get_rain_wind());
data.concat("&softwaretype=vws%20versionxx&action=updateraw");

if(SIM800l.checkNetworkRegistration()==0)
httpPost(data);

}

