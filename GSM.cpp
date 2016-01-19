#include "serialib.h"
#include "functions.h"

int main()
{
	cout << "Start of the program" << endl;
	//Every AT command starts with "AT", sync baud rate
	string msgTxtMode="AT+CMGF=1\n";             	// Set SMS message command as text mode
	string readMsg="AT+CMGR=1\n";        			// Command for reading message from location 1 from inbox
	string deleteMessageByPosition="AT+CMGD=1\n";   // Erasing all wrote messages from inbox
	string ctrlD="\x1A";							// Symbols which are equivalent to pressing Ctrl + D buttons
	//char* checkMessageStatus="AT+CMGL=\"ALL\"";   // Check status of received SMS
	string k1 = "\"";
	string bEnter = "\r";							// Symbols which are equivalent to pressing Enter button
	string msgFail = "Command incorrect!\x1A";						 // String creation
	string confirm = "Login accepted!\x1A";					// Text of string that will send
	double T;
	string strT, telNumber, msg, word1, word2, word3, word4, userName;

	ADC_UART_enabling();							// Initialization of ADC and UART
	send(msgTxtMode);							// Set text mode on GSM M95 click
	send(deleteMessageByPosition);					// SMS deleting

	while (true) {

		// Reading of SMS
		string *recvarray;
		recvarray = send(readMsg);					// Reading SMS to string answ
		word1 = *(recvarray+0);
		int lenOfMsg1 = strlen(word1.c_str());
		cout << *(recvarray) <<*(recvarray+1) <<*(recvarray+4) << endl;

	/*	if ((lenOfMsg1 != 0) && (*(recvarray+0) != "L")) {
			string telNumber = "AT+CMGS=" + *(recvarray+4) + bEnter;	// Write telephone number to string telNumber
			cout << telNumber << "inside kek" << endl;
			//send(telNumber);
			//send(msgFail);											// Send warning message to user
			send(deleteMessageByPosition);
		}*/
		cout << *(recvarray) <<*(recvarray+1) <<*(recvarray+4) << endl;
		// Initialization(log in) of user
		if (*(recvarray+0)=="L"){												// If first word of SMS is "L" ...
			userName = *(recvarray+1);									// Write second word(name of user) to string userName
			telNumber = "AT+CMGS=" + *(recvarray+4) + bEnter;	// Write third word(telephone number) to string telNumber

			//send(telNumber);										// Enter of telephone number to GSM module
			cout << "Login accepted!" << endl;
			//send(confirm);											// Send string which contains SMS with text of login confirmation
			send(deleteMessageByPosition);
			// Working cycle of program which includes modes
			bool k1 = true;
			while (k1 == true){

				recvarray = send(readMsg);
				word1 = *(recvarray+0);
				int lenOfMsg = strlen(word1.c_str());
				cout << *(recvarray+0) << "==" << userName << endl;
				cout << lenOfMsg << "==" << "0" << endl;
			/*	if ((*(recvarray+0) != userName) && (lenOfMsg != 0))  {
							telNumber = "AT+CMGS=" + *(recvarray+4) + bEnter;	// Write telephone number to string telNumber
							cout << telNumber << endl;
							//send(telNumber);
							//send(msgFail);											// Send warning message to user
							cout << "message sent" << endl;
							send(deleteMessageByPosition);
				}*/

					// Check status
					if ((*(recvarray+0) == userName) && (*(recvarray+1) == "status")){	// Chech SMS for "Username" and "status"
						send(deleteMessageByPosition);				// Clear message buffer
						T = T_ADC_reading();					// Reading of ADC value via function
						cout << T << " - Temperature" << endl;
						strT = convTtoMsg(T); // Temperature conversion from double to string
						msg = "Current temperature: " + strT + ctrlD;  // Creation of string for sending
						// Sending of SMS with current temperature
						send(telNumber);			// Entering of tel. number to GMS module
						send(msg);					// Sending of string that was recently created
						cout << msg << endl;
						cout << "Temperature sent by status 1" << endl;

					}
					// Log out from system
					if ((*(recvarray+0) == userName) && (*(recvarray+1) == "LO")){
						send(deleteMessageByPosition);
						msg = "Exit from system have completed\x1A"; // String creation
						send(telNumber);
						send(msg);			// Send warning message to user
						cout << msg << endl;
						k1 = false;
					}
					// Enabling of tracking mode
					if ((*(recvarray+0) == userName) && (*(recvarray+1) == "T")){		// Chech SMS for "Username" and "T"
						send(deleteMessageByPosition);    // Clear message buffer
						bool k2 = true;

						//Conversion of strings with temperature range to double type
						string tmin = *(recvarray+2);
						string tmax = *(recvarray+3);
						double tMin = atof(tmin.c_str());		// Convert third word to tMin as double
						double tMax = atof(tmax.c_str());		// Convert fourth word to tMax as double
						cout << tMin << " - tMin" << endl;
						cout << tMax << " - tMax" << endl;


						while (k2 == true){

							// Reading and splitting of SMS (described above)
							send(msgTxtMode);
							recvarray = send(readMsg);
							T = T_ADC_reading();
							cout << T << " - Current T" << endl;
							// Check on temperature conformity condition
							word1 = *(recvarray+0);
							int lenOfMsg = strlen(word1.c_str());
						/*	if ((*(recvarray+0) != userName) && (lenOfMsg != 0))  {
								telNumber = "AT+CMGS=" + *(recvarray+4) + bEnter;	// Write telephone number to string telNumber
								send(telNumber);
								send(msgFail);											// Send warning message to user
								send(deleteMessageByPosition);
												}*/
							if ((T < tMin) || (T > tMax)){
								send(deleteMessageByPosition);
								// Temperature conversion from double to string (described above)
								strT = convTtoMsg(T); // Temperature conversion from double to string
								msg = "Warning! Current temperature:  " + strT + ctrlD; // String creation
								send(telNumber);
								send(msg);			// Send warning message to user
								cout << "Temperature sent by T command" << endl;

								k2 = false;
							}
							// Check of current temperature in tracking mode
							if ((*(recvarray+0) == userName) && (*(recvarray+1) == "status")){
								send(deleteMessageByPosition);
								double T = T_ADC_reading();
								cout << T << endl;
								cout << telNumber << endl;
								strT = convTtoMsg(T); // Temperature conversion from double to string
								msg = "Current temperature: " + strT + ctrlD;
								send(telNumber);
								send(msg);
								cout << "Temperature sent by status2" << endl;

							}
							//End of cycle without SMS deleting in case of new boundaries
							if ((*(recvarray+0) == userName) && (*(recvarray+1) == "T")){
								k2 = false;
							}
							//End of cycle
							if ((*(recvarray+0) == userName) && (*(recvarray+1) == "ET")){
								send(deleteMessageByPosition);
								msg = "Tracking mode have turned off\x1A"; // String creation
								send(telNumber);
								send(msg);			// Send warning message to user
								k2 = false;
							}
							// Log out from system
							if ((*(recvarray+0) == userName) && (*(recvarray+1) == "LO")){
								send(deleteMessageByPosition);
								msg = "Exit from system have completed\x1A"; // String creation
								send(telNumber);
								send(msg);			// Send warning message to user

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



