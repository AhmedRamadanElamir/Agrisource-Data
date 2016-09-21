void communicate(){
	 while(Serial1.available())
	 Serial.write(Serial1.read());
    }
void setup() {
  //Begin serial comunication with Arduino and Arduino IDE (Serial Monitor)
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

  Serial.println("Setup Complete!");
  Serial.println("Sending SMS...");
   
  //Set SMS format to ASCII
  Serial1.println("AT+CMGF=1\r\n");
  delay(1000);
 
  //Send new SMS command and message number
  Serial1.println("AT+CMGS=\"01146808317\"\r\n");
  delay(1000);
   
  //Send SMS content
  Serial1.println("!!Hello World!!");
  delay(1000);
   
  //Send Ctrl+Z / ESC to denote SMS message is complete
  Serial1.println((char)26);
  delay(1000);
     
  Serial.println("SMS Sent!");
}
 
void loop() {
}

