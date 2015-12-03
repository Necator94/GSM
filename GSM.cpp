#include <stdio.h>
#include "serialib.h"
#include "functions.h"
#include "unistd.h"


//char* baudRate="AT"                        		//Every AT command starts with "AT", sync baudrade
string disableEcho="ATE1\n";					  		// Disable command line echo
string msgTxtMode="AT+CMGF=1\n";                        // Set sms message command as text mode
string sms="AT+CMGS=\"+491748075141\"\r";				// Sending SMS to desired number
string readMsg="AT+CMGR=1\n";          // Command for reading message from location 1 from inbox
string deleteMessageByPosition="AT+CMGD=1\n";         // Erasing all wrote messages from inbox
string ctrlD="\x1A";
//char* checkMessageStatus="AT+CMGL=\"ALL\"";        // Check status of received SMS
string k1 = "\"";
string bEnter = "\r";

int main()
{

	ADC_UART_enabling();
	//send(deleteMessageByPosition);
	while (true){

		send(msgTxtMode);
		string answ = send(readMsg);
		string w1, w2, w3;
		fun(&answ, &w1, &w2, &w3);
		cout << "msg content   "<< answ << w1 << w2 << endl;
		if (answ=="L"){
			string userName = w1;
			string telNumber = "AT+CMGS=" + k1 + w2 + k1 + bEnter;
			send(telNumber);
			string sup = "Login accepted!\x1A";
			cout << "Login accepted" << endl;
			send(sup);


			bool k1 = true;
			while (k1 == true){
				send(msgTxtMode);
				string answ = send(readMsg);
				string w1, w2, w3;
				fun(&answ, &w1, &w2, &w3);

					if ((answ == userName) && (w1 == "status")){
						float T = T_ADC_reading(); //// re!!!!!!!!!!!!!!!!!!!!1
						cout << T << endl;
						cout << telNumber << endl;
						stringstream strRt;
						string strT;
						strRt << T;
						strRt >> strT;
						string msg = "Current temperature: " + strT + ctrlD;
						send(telNumber);
						send(msg);
						cout << "Temperature sent by status1" << endl;
						sleep(5);
					}

					if ((answ == userName) && (w1 == "T")){
						bool k2 = true;
						while (k2 == true){
						float T = T_ADC_reading();

							if ((T < w2) || (T > w3)){
								stringstream strRt1;
								string strT1;
								strRt1 << T;
								strRt1 >> strT1;
								string msg = "Warning! Current temperature:  " + strT1 + ctrlD;
								send(telNumber);
								send(msg);
								cout << "Temperature sent by status1" << endl;
								sleep(5);

								}
						}
					}



			//send(deleteMessageByPosition);
			}
		}

/*
		if (answ=="status"){
			string T = T_ADC_reading();
			cout << T << endl;
			send(sms);
			send(T);
		}
		//send(deleteMessageByPosition);
		 *
*/
	}
	return 0;
}



