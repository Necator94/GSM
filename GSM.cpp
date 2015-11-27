#include <stdio.h>
#include "serialib.h"
#include "functions.h"


//char* baudRate="AT"                        		//Every AT command starts with "AT", sync baudrade
char* disableEcho="ATE1\n";					  		// Disable command line echo
char* msgTxtMode="AT+CMGF=1\n";                        // Set sms message command as text mode
char* sms="AT+CMGS=\"+491748075141\"\r";				// Sending SMS to desired number
char* readMsg="AT+CMGR=1\n";          // Command for reading message from location 1 from inbox
char* deleteMessageByPosition="AT+CMGD=1\n";         // Erasing all wrote messages from inbox
char* text="1word 2word 3word \x1A";
//char* checkMessageStatus="AT+CMGL=\"ALL\"";        // Check status of received SMS


int main()
{

	ADC_UART_enabling();

	//send(status);
	float T = T_ADC_reading();
	cout << T << endl;
	//send(deleteMessageByPosition);

while (T > 0){

	send(msgTxtMode);
	string answ = send(readMsg);
	string w1, w2;
	fun(&answ, &w1, &w2);
	if (answ=="status"){
		T = T_ADC_reading();
		char *ca;
		cout << "che blya" << endl;
		sprintf(ca, "%f", T);
		//char *tt=T;
		//send(ca);
		//send(deleteMessageByPosition);
		cout<<ca<<endl;
	}
	T = T_ADC_reading();
}

		//send(sms);
	//;
   //










return 0;
}

