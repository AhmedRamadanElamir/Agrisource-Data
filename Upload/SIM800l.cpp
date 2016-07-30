#include "SIM800l.h"

SIM800l::SIM800l(HardwareSerial *ser)
{
	GSM = ser;
}

uint8_t SIM800l::init(unsigned long baudRate)
{
	GSM->begin(baudRate);
	initializeGSM();
	gsmSleepMode(2);
}


void SIM800l::initializeGSM()
{
	GSM->println("AT+CMEE=1");
	confirmAtCommand("OK",CMEE_TO);
	GSM->println("AT+IPR=9600");
	confirmAtCommand("OK",IPR_TO);
	GSM->println("AT+CMGF=1");
	confirmAtCommand("OK",CMGF_TO);
	GSM->println("AT+CNMI=0,0,0,0,0");
	confirmAtCommand("OK",CNMI_TO);
}

uint8_t SIM800l::gsmSleepMode(int mode)
{
	GSM->println("AT\r\nAT");
	delay(50);
	GSM->print("AT+CSCLK=");
	GSM->println(mode);
	return(confirmAtCommand("OK",CSCLK_TO));
}

uint8_t SIM800l::confirmAtCommand(char *searchString, unsigned long timeOut)
{
	uint8_t index = 0;
	unsigned long tOut = millis();
	while((millis() - tOut) <= timeOut)
	{
        GSM->flush();
		if (GSM->available())
		{
			atRxBuffer[index] = GSM->read();
			index++;
			atRxBuffer[index] = '\0';
			if(strstr(atRxBuffer,searchString) != NULL)
				return 0;
			if(strstr(atRxBuffer,"ERROR") != NULL)
				return 1;
			if(index >= (INDEX_SIZE - 1))
				return 2;	
		}
	}
	return 3;
}

uint8_t SIM800l::checkNetworkRegistration()
{
	GSM->println("AT+CREG?");
	confirmAtCommand("OK",CREG_TO);
	if((strstr(atRxBuffer,",1") != NULL) || (strstr(atRxBuffer,",5") != NULL))
		return 0;
	return 1;
}

void SIM800l::httpPost(String data)	
{
	GSM->gsmSleepMode(0);
	
	Serial->println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
	GSM->confirmAtCommand("OK",5000);
	
	Serial->println("AT+SAPBR=3,1,\"APN\",\"\""); //need to put your provider's APN here
	GSM->confirmAtCommand("OK",5000);
	
	Serial->println("AT+SAPBR=1,1");
	GSM->confirmAtCommand("OK",5000);// Tries to connect GPRS 
		
	Serial->println("AT+HTTPINIT");
	GSM->confirmAtCommand("OK",5000);
	
	Serial->println("AT+HTTPPARA=\"CID\",1");
	GSM->confirmAtCommand("OK",5000);
	
	Serial->println("AT+HTTPPARA=\"URL\",\"https://weatherstation.wunderground.com/weatherstation/updateweatherstation.php\""); //web address to send data to
	GSM->confirmAtCommand("OK",5000);
	
	Serial->println("AT+HTTPDATA=200,10000"); //100 refers to how many bytes you're sending.  You'll probably have to tweak or just put a large #
	GSM->confirmAtCommand("DOWNLOAD",5000);
	
	Serial->println(data); 
	GSM->confirmAtCommand("OK",5000);
	
	Serial->println("AT+HTTPACTION=1"); //POST the data
	GSM->confirmAtCommand("ACTION:",5000);
	
	Serial->println("AT+HTTPTERM"); //terminate http
	GSM->confirmAtCommand("OK",5000);
	
	Serial->println("AT+SAPBR=0,1");// Disconnect GPRS
	GSM->confirmAtCommand("OK",5000);
	GSM->confirmAtCommand("DEACT",5000);
	
	GSM->gsmSleepMode(2);
	}






















