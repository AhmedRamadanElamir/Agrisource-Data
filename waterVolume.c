volatile int NbTopsFan; //measuring the rising edges of the signal
int Calc;
int hallsensor = 2; //The pin location of the sensor
int waterUsed = 0; 

void rpm () //This is the function that the interupt calls
{
 NbTopsFan++; //This function measures the rising and falling edge of the hall effect sensors signal
}

void setup() 
{
 pinMode(hallsensor, INPUT); //initializes digital pin 2 as an input
 Serial.begin(9600); //This is the setup function where the serial port is initialised
 attachInterrupt(0, rpm, RISING); //and the interrupt is attached
}

void loop ()
{
 NbTopsFan = 0; //Set NbTops to 0 ready for calculations
 sei();    //Enables interrupts
 delay (1000); //Wait 1 second
 cli();    //Disable interrupts
 Calc = (NbTopsFan * 60 / 7.5); //(Pulse frequency x 60) / 7.5Q, = flow rate in L/hour
waterUsed += Calc*0.000277777;
Serial.print(waterUsed, DEC);
Serial.print("L\r\n");
delay(1000); 
}
