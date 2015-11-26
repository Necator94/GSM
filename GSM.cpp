#include <stdio.h>
#include "serialib.h"
#include "functions.h"


//char* baudRate="AT"                        		//Every AT command starts with "AT", sync baudrade
char* disableEcho="ATE1\n";					  		// Disable command line echo
char* MsgTxtMode="AT+CMGF=1\n";                        // Set sms message command as text mode
char* sms="AT+CMGS=\"+491748075141\"\r";				// Sending SMS to desired number
char* readMsg="AT+CMGR=1\n";          // Command for reading message from location 1 from inbox
char* deleteMessageByPosition="AT+CMGD=1\n";         // Erasing all wrote messages from inbox
char* text="kek suka pizda\x1A";
//char* checkMessageStatus="AT+CMGL=\"ALL\"";        // Check status of received SMS


int main(){

	ADC_UART_enabling();
	//send(status);

	send(MsgTxtMode);
	string answ=send(readMsg);
	string w1,w2;
	string kek=fun(&answ,&w1,&w2);
	cout << answ << endl;
	cout << w1 << endl;
	cout << w2 << endl;
	//send(sms);
	//send(text);
   // send(deleteMessageByPosition);










return 0;
}

