#ifndef SIM800l_h
#define SIM800l_h

#define CSCLK_TO		2000L   
#define CMEE_TO			10000L  
#define CMGF_TO			10000L 
#define CNMI_TO			10000L  
#define CREG_TO			30000L  
#define INDEX_SIZE		100
#define OK				"OK"


class SIM800l 
{
	public:
		char atRxBuffer[INDEX_SIZE];
		uint8_t init(unsigned long);
		uint8_t gsmSleepMode(int);
		uint8_t checkNetworkRegistration();
		uint8_t confirmAtCommand(char *, unsigned long);
		void httpPost(String data);

	private:
		void initializeGSM();
};

#endif
