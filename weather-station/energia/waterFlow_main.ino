#include <Energia.h>
volatile int NbTopsFan; //measuring the rising edges of the signal
int Calc;                               
#define hallsensor PD_0    //The pin location of the sensor
int data = 0;
int counter = 0; //counter to upload each 12 hours

void setup()
{ 
pinMode(hallsensor, INPUT); 
Serial.begin(9600); 
delay(1000);
Serial1.begin(9600);
attachInterrupt(hallsensor, rpm, RISING); 
} 

void loop(){ 
calc_data();
if(counter = 43200){
 send();
 counter = 0;
}
}

void send(){
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

String reading = "AT+HTTPPARA=\"URL\",\"agrisourcedata.com/sensors/modem.php?FID=14&SID=1&val1="; 
reading.concat(data);	
reading.concat("\"");
Serial1.println(reading); //web address to send data to
	delay(200);
communicate();

Serial1.println("AT+HTTPACTION=0"); //Get request
	delay(200);
communicate();
Serial1.println("AT+HTTPREAD"); //read data
	delay(200);
communicate();

Serial1.println("AT+HTTPTERM"); //terminate http session
	delay(200);
communicate();
}


void communicate(){
	 while(Serial1.available())
	 Serial.write(Serial1.read()); 
}

void calc_data(){
 NbTopsFan = 0;   //Set NbTops to 0 ready for calculations
  interrupts();      //Enables interrupts
  delay (1000);   //Wait 1 second
  noInterrupts();      //Disable interrupts
  Calc = (NbTopsFan / 7.5); //(Pulse frequency) / 7.5Q, = flow rate in L/min
data = data + Calc;
counter = counter +1;
delay(60000);
}

void rpm ()     //This is the function that the interupt calls 
{ 
  NbTopsFan++;  //This function measures the rising and falling edge of the hall effect sensors signal
} 
