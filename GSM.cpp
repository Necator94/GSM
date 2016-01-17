#include <stdio.h>
#include "serialib.h"
#include "functions.h"
#include "unistd.h"




int main()
{

	//Every AT command starts with "AT", sync baud rate
	string msgTxtMode="AT+CMGF=1\n";             	// Set SMS message command as text mode
	string sms="AT+CMGS=\"+491748075141\"\r";		// Sending SMS to desired number
	string readMsg="AT+CMGR=3\n";        			// Command for reading message from location 1 from inbox
	string deleteMessageByPosition="AT+CMGD=1\n";   // Erasing all wrote messages from inbox
	string ctrlD="\x1A";							// Symbols which are equivalent to pressing Ctrl + D buttons
	//char* checkMessageStatus="AT+CMGL=\"ALL\"";   // Check status of received SMS
	string k1 = "\"";
	string bEnter = "\r";							// Symbols which are equivalent to pressing Enter button
	cout << "Start of the program" << endl;
	ADC_UART_enabling();							// Initialization of ADC and UART
	//send(deleteMessageByPosition);					// SMS deleting


	while (true){

		// Reading of SMS
		send(msgTxtMode);							// Set text mode on GSM M95 click
	//	string w1, w2, w3, w4, w5;
		string w1, w2, w3, w4, w5 = send(readMsg);				// Reading SMS to string answ
		//send(readMsg);
		//	Splitting of SMS
//		string w1, w2, w3;							//Initialization of variables
//		splitstr(&answ, &w1, &w2, &w3);					// Split string with spaces on variables via fun function
//		cout << "msg content   "<< w1 <<" " << w2 << " " << w3 << " " << w4 << " " << w5  << endl;
		string answ = "qee";
		// Initialization(log in) of user
		if (answ=="L"){												// If first word of SMS is "L" ...
			string userName = w1;									// Write second word(name of user) to string userName
			string telNumber = "AT+CMGS=" + k1 + w2 + k1 + bEnter;	// Write third word(telephone number) to string telNumber
			send(telNumber);										// Enter of telephone number to GSM module
			string confirm = "Login accepted!\x1A";					// Text of string that will send
			cout << "Login accepted" << endl;
			send(confirm);											// Send string which contains SMS with text of login confirmation
			send(deleteMessageByPosition);

			// Working cycle of program which includes modes
			bool k1 = true;
			while (k1 == true){

				// Reading and splitting of SMS (described above)
				send(msgTxtMode);
				string answ = send(readMsg);
				string w1, w2, w3;
//				splitstr(&answ, &w1, &w2, &w3);

					// Check status
					if ((answ == userName) && (w1 == "status")){	// Chech SMS for "Username" and "status"
						double T = T_ADC_reading();					// Reading of ADC value via function
						cout << T << endl;
						cout << telNumber << endl;

						// Temperature conversion from double to string
						stringstream strRt;			 // Conversion is completed via stringstream class
						string strT;
						strRt << T;
						strRt >> strT;				// Variable strT contains temperature as a string type

						// Sending of SMS with current temperature
						string msg = "Current temperature: " + strT + ctrlD;  // Creation of string for sending
						send(telNumber);			// Entering of tel. number to GMS module
						sleep(1);					// Pause
						send(msg);					// Sending of string that was recently created
						sleep(1);
						cout << "Temperature sent by status1" << endl;
						send(deleteMessageByPosition);	// Clear message buffer
						sleep(1);
					}
					// Log out from system
					if ((answ == userName) && (w1 == "LO")){
						string msg = "Exit from system have completed\x1A"; // String creation
						send(telNumber);
						sleep(1);
						send(msg);			// Send warning message to user
						sleep(1);
						send(deleteMessageByPosition);
						k1 = false;
					}
					// Enabling of tracking mode
					if ((answ == userName) && (w1 == "T")){		// Chech SMS for "Username" and "T"
						bool k2 = true;

						// Reading and splitting of SMS (described above)
						send(msgTxtMode);
						answ = send(readMsg);
	//					splitstr(&answ, &w1, &w2, &w3);
						//Conversion of strings with temperature range to double type
						double tMin = atof(w2.c_str());		// Convert third word to tMin as double
						double tMax = atof(w3.c_str());		// Convert fourth word to tMax as double
						cout << tMin << " - tMin" << endl;
						cout << tMax << " - tMax" << endl;
						send(deleteMessageByPosition);    // Clear message buffer

						while (k2 == true){				  // Until k2 != false

							// Reading and splitting of SMS (described above)
							send(msgTxtMode);
							answ = send(readMsg);
	//						splitstr(&answ, &w1, &w2, &w3);
							cout << "msg content   "<< answ << w1 << w2 << endl;


							double T = T_ADC_reading();
							cout << T << " - Current T" << endl;
							sleep(1);
							// Check on temperature conformity condition
							if ((T < tMin) || (T > tMax)){
								// Temperature conversion from double to string (described above)
								stringstream strRt1;
								string strT1;
								strRt1 << T;
								strRt1 >> strT1;

								string msg = "Warning! Current temperature:  " + strT1 + ctrlD; // String creation
								send(telNumber);
								sleep(1);
								send(msg);			// Send warning message to user
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
							//End of cycle without SMS deleting in case of new boundaries
							if ((answ == userName) && (w1 == "T")){
								k2 = false;
							}
							//End of cycle
							if ((answ == userName) && (w1 == "ET")){
								string msg = "Tracking mode have turned off\x1A"; // String creation
								send(telNumber);
								sleep(1);
								send(msg);			// Send warning message to user
								sleep(1);
								send(deleteMessageByPosition);
								k2 = false;
							}
							// Log out from system
							if ((answ == userName) && (w1 == "LO")){
								string msg = "Exit from system have completed\x1A"; // String creation
								send(telNumber);
								sleep(1);
								send(msg);			// Send warning message to user
								sleep(1);
								send(deleteMessageByPosition);
								k2 = false;
								k1 = false;
							}
						}
					}
			}
		}
	sleep(1);
	}
	return 0;
}



