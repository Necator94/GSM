#include <stdio.h>
#include "serialib.h"
#include "functions.h"
#include <cstring>
#include <vector>
#include <string>
#include <sstream>
string status="AT\n";
//char* baudRate="AT"                        		//Every AT command starts with "AT", sync baudrade
char* disableEcho="ATE1\n";					  		// Disable command line echo
char* MsgTxtMode="AT+CMGF=1\n";                        // Set sms message command as text mode
char* sms="AT+CMGS=\"+491748075141\"\r";				// Sending SMS to desired number
char* readMsg="AT+CMGR=1\n";          // Command for reading message from location 1 from inbox
char* deleteMessageByPosition="AT+CMGD=1\n";         // Erasing all wrote messages from inbox
char* text="kek suka pizda\x1A";
//char* checkMessageStatus="AT+CMGL=\"ALL\"";        // Check status of received SMS


int main(){



	//send(status);

	send(MsgTxtMode);
	string answ=send(readMsg);
	cout<<answ<<endl;
	//send(sms);
	//send(text);

    string buf; // Have a buffer string
    stringstream ss(answ); // Insert the string into a stream
    vector<string> tokens; // Create vector to hold our words
    char b[20];
    while (ss >> buf){
       int i=0;
    	tokens.push_back(buf);
       b[i]=buf;
       i++;
cout<<b[i]<<endl;
    }






return 0;
}

