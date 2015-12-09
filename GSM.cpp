#include <stdio.h>
#include "serialib.h"
#include "functions.h"
#include "unistd.h"




int main()
{

	//Every AT command starts with "AT", sync baud rate

	string msgTxtMode="AT+CMGF=1\n";                        		// Set SMS message command as text mode
	string sms="AT+CMGS=\"+491748075141\"\r";						// Sending SMS to desired number
	string readMsg="AT+CMGR=1\n";        							// Command for reading message from location 1 from inbox
	string deleteMessageByPosition="AT+CMGD=1\n";         			// Erasing all wrote messages from inbox

	string ctrlD="\x1A";											// Symbols which are equivalent to pressing Ctrl + D buttons
	//char* checkMessageStatus="AT+CMGL=\"ALL\"";        			// Check status of received SMS
	string k1 = "\"";
	string bEnter = "\r";											// Symbols which are equivalent to pressing Enter button











	ADC_UART_enabling();															// Initialization of ADC and UART
	send(deleteMessageByPosition);													// SMS deleting

	double T = T_ADC_reading();



	while (true){

		// Reading of SMS
		send(msgTxtMode);															// Set text mode on GSM M95 click
		string answ = send(readMsg);												// Reading SMS to string answ

		//	Splitting of SMS
		string w1, w2, w3;
		fun(&answ, &w1, &w2, &w3);													// Split string on variables via fun function
		cout << "msg content   "<< answ << w1 << w2 << endl;

		// Initialization(log in) of user
		if (answ=="L"){																// If first word of SMS is "L" ...
			string userName = w1;													// Write second word(name of user) to string userName
			string telNumber = "AT+CMGS=" + k1 + w2 + k1 + bEnter;					// Write third word(telephone number) to string telNumber
			send(telNumber);														// Enter of telephone number to GSM module
			string confirm = "Login accepted!\x1A";
			cout << "Login accepted" << endl;
			send(confirm);															// Send string which contains SMS with text of login confirmation
			send(deleteMessageByPosition);

			// Working cycle of program which includes modes
			bool k1 = true;
			while (k1 == true){

				// Reading and splitting of SMS
				send(msgTxtMode);
				string answ = send(readMsg);
				string w1, w2, w3;
				fun(&answ, &w1, &w2, &w3);

					// Status mode
					if ((answ == userName) && (w1 == "status")){
						double T = T_ADC_reading();									// Reading of ADC value via function
						cout << T << endl;
						cout << telNumber << endl;

						// Temperature conversion from double to string
						stringstream strRt;
						string strT;
						strRt << T;
						strRt >> strT;

						// Sending of SMS with current temperature
						string msg = "Current temperature: " + strT + ctrlD;
						send(telNumber);
						sleep(1);
						send(msg);
						sleep(1);
						cout << "Temperature sent by status1" << endl;
						send(deleteMessageByPosition);
						sleep(1);
					}

					// Tracking mode
					if ((answ == userName) && (w1 == "T")){
						bool k2 = true;

						// Reading and splitting of SMS
						send(msgTxtMode);
						answ = send(readMsg);
						fun(&answ, &w1, &w2, &w3);
						//Conversion of strings with temerature range to double type
						double tMin = atof(w2.c_str());
						double tMax = atof(w3.c_str());
						cout << tMin << " - tMin" << endl;
						cout << tMax << " - tMax" << endl;
						send(deleteMessageByPosition);

						while (k2 == true){											// Until SMS with warning will be send

							send(msgTxtMode);
							answ = send(readMsg);
							fun(&answ, &w1, &w2, &w3);
							cout << "msg content   "<< answ << w1 << w2 << endl;


							double T = T_ADC_reading();
							cout << T << " - Current T" << endl;
							sleep(1);
							// Check on temperature conformity condition
							if ((T < tMin) || (T > tMax)){
								stringstream strRt1;
								string strT1;
								strRt1 << T;
								strRt1 >> strT1;
								string msg = "Warning! Current temperature:  " + strT1 + ctrlD;
								send(telNumber);
								sleep(1);
								send(msg);
								sleep(1);
								cout << "Temperature sent by T command" << endl;
								send(deleteMessageByPosition);
								k2 = false;
								sleep(1);
							}
							// Check of current temperature in tracking mode
							if ((answ == userName) && (w1 == "status")){
								double T = T_ADC_reading();
								cout << T << endl;
								cout << telNumber << endl;
								stringstream strRt3;
								string strT3;
								strRt3 << T;
								strRt3 >> strT3;
								string msg = "Current temperature: " + strT3 + ctrlD;
								send(telNumber);
								sleep(1);
								send(msg);
								sleep(1);
								cout << "Temperature sent by status2" << endl;
								send(deleteMessageByPosition);
								sleep(1);
							}

							if ((answ == userName) && (w1 == "T")){
							k2 = false;


							}

						}
					}
			}
		}
	sleep(1);
	}
	return 0;
}



