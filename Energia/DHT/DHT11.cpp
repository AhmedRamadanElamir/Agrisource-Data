#include <Energia.h>
#include "DHT11.h"

#define  DHpin PF_4

char dat[5];

fclass::fclass(){
}

void fclass::dht_init(){
	pinMode(DHpin,OUTPUT);
  	delay(1000);
}

char fclass::read_data(){    //Sensor detection
  		char data;
    	data=0x00;
    	for(int i=0; i<8; i++)
    	{
    		if(digitalRead(DHpin) == LOW)
    		{
    			while(digitalRead(DHpin) == LOW);
    			delayMicroseconds(30);
    			if(digitalRead(DHpin) == HIGH)
    				data |= (1<<(7-i));
    			while(digitalRead(DHpin) == HIGH);
    		}
    	}
    	return data;	 
}

void fclass::start_test(){
    digitalWrite(DHpin,LOW);
	delay(30);
	digitalWrite(DHpin,HIGH);
	delayMicroseconds(40);
	pinMode(DHpin,INPUT);
	while(digitalRead(DHpin) == HIGH);
	delayMicroseconds(80); //
	if(digitalRead(DHpin) == LOW);
	delayMicroseconds(80);
	for(int i=0;i<4;i++)
		dat[i] = read_data();
	pinMode(DHpin,OUTPUT);
	digitalWrite(DHpin,HIGH);
}

char fclass::get_Temp(){    //Sensor detection
  return dat[2];
}

char fclass::get_Hum(){    //Sensor detection
	return dat[0];
}


fclass dht = fclass();





