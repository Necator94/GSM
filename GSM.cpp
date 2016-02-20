#include "serialib.h"
#include "functions.h"

int main()
{
	cout << "Start of the program" << endl;
	//Every AT command starts with "AT", sync baud rate
	string msgTxtMode="AT+CMGF=1\n";             			// Set SMS message command as text mode
	string readMsg="AT+CMGR=1\n";        					// Command for reading message from location 1 from inbox
	string deleteMessageByPosition="AT+CMGD=1\n";   		// Erasing all wrote messages from inbox
	string ctrlD="\x1A";									// Symbols which are equivalent to pressing Ctrl + D buttons
	//char* checkMessageStatus="AT+CMGL=\"ALL\"";   		// Check status of received SMS
//	string k1 = "\"";
	string bEnter = "\r";									// Symbols which are equivalent to pressing Enter button
	string msgFail = "Command incorrect!\x1A";				// String creation
	string confirm = "Login accepted!\x1A";					// Text of string that will send
	double T;
	string strT, telNumber, msg, word1, word2, word3, word4, userName, word1check, *recvarray;
	bool ncheck = false;
	bool k1, k2;
	int lenOfString;
	ADC_UART_enabling();									// Initialization of ADC and UART
	send(msgTxtMode);										// Set text mode on GSM M95 click
	send(deleteMessageByPosition);							// SMS deleting

	int counter0 = 0;
	string gpioArg = "48";									// Number of GPIO for Enabling
	ofstream adc_on0 ("/sys/class/gpio/export");			// Opening of GPIO export file
	adc_on0 << gpioArg;										// Write command to file
	adc_on0.close();										// Close file
	gpioArg = "out";										// direction for GPIO port
	ofstream adc_on1 ("/sys/class/gpio/gpio48/direction");	// Opening of GPIO direction file
	adc_on1 << gpioArg;										// Write command to file
	adc_on1.close();	 									// Close file
	gpioArg = "1";											// High level of voltage on GPIO pin
	ofstream adc_on2 ("/sys/class/gpio/gpio48/value");		// Opening of GPIO value file
	adc_on2 << gpioArg;										// Write command to file
	adc_on2.close();

	while (true) {
		ncheck = false;										// Set flag (true value means correct receiving and reading of SMS string)
		// Reading of SMS
		recvarray = send(readMsg);							// Reading of GSM module response (returned value - pointer to array)
		word1 = *(recvarray+0);								// Assign value of first word to variable ()
		sleep(1);											// Pause is necessary for identification difference between reading commands
		recvarray = send(readMsg);							// Reading of GSM module response
		word1check = *(recvarray+0);						// Assign value of first word to variable ()
		lenOfString = strlen(word1.c_str());
		cout << *(recvarray) <<*(recvarray+1) <<*(recvarray+4) << endl;
		if (word1check == word1) {							// If two times read SMS is equal, flag is true
			ncheck = true;
		}

		if ((lenOfString != 0) && (*(recvarray+0) != "L") && (ncheck = true))  {	// If 1st word is not empty & 1st word if "L" & SMS was read correctly
			sleep(1);
			send(deleteMessageByPosition);											// Delete last received SMS
			telNumber = "AT+CMGS=" + *(recvarray+4) + bEnter;						// Write telephone number to string telNumber
			cout << telNumber << "inside kek" << endl;
			sleep(1);
			send(telNumber);														// Send command for entering of tel. number (string with number included)
			sleep(1);
			send(msgFail);															// Send warning message to user (Command is not correct!)
			sleep(1);
		}

		// Initialization(log in) of user
		if (*(recvarray+0)=="L"){													// If first word of SMS is equal to "L"
			sleep(1);
			send(deleteMessageByPosition);											// Delete last received SMS
			sleep(1);
			userName = *(recvarray+1);												// Write second word(name of user) to string userName
			telNumber = "AT+CMGS=" + *(recvarray+4) + bEnter;						// Write third word(telephone number) to string telNumber
			int lenO = strlen(userName.c_str());
			cout << lenO << " length of -" << userName << endl;
			send(telNumber);
			cout << "Login accepted!" << endl;
			sleep(1);
			send(confirm);
			sleep(1);

			// Working cycle of program which includes modes
			k1 = true;																// Status true allows execution of wait-for-command cycle
			while (k1 == true){
				sleep(1);
				ncheck = false;
				recvarray = send(readMsg);
				word1 = *(recvarray+0);
				sleep(1);
				recvarray = send(readMsg);
				word1check = *(recvarray+0);
				lenOfString = strlen(word1.c_str());
				int lenO = strlen(userName.c_str());
				cout << *(recvarray+0) << "==" << userName << endl;
				cout << lenOfString << "!=" << "0" << endl;
				cout << lenO << "user length" << endl;
				if (word1check == word1) {
					ncheck = true;
				}
				if ((*(recvarray+0) != userName) && (lenOfString != 0) && (ncheck = true) ||
				   ((*(recvarray+1) != "T") && (*(recvarray+1) != "status") && (*(recvarray+1) != "LO")) && (lenOfString != 0) && (ncheck = true))  {
					sleep (1);
					send(deleteMessageByPosition);
					telNumber = "AT+CMGS=" + *(recvarray+4) + bEnter;
					cout << telNumber << endl;
					sleep(1);
					send(telNumber);
					sleep(1);
					send(msgFail);
					sleep(1);
					cout << "message sent" << endl;

				}
				// Check status
				if ((*(recvarray+0) == userName) && (*(recvarray+1) == "status")){					// if the first word is equal to username and the second word (command) is equal to "status"
					sleep(1);
					send(deleteMessageByPosition);
					sleep(1);
					T = T_ADC_reading();															// Reading of ADC value via function
					cout << T << " - Temperature" << endl;
					strT = convTtoMsg(T); 															// Temperature conversion from double to string via function
					msg = "Current temperature: " + strT + ctrlD;  									// Creation of string for sending (current temperature included)
					// Sending of SMS with current temperature
					send(telNumber);
					sleep(1);
					send(msg);
					sleep(1);
					cout << msg << endl;
					cout << "Temperature sent by status 1" << endl;
				}
				// Log out from system
				if ((*(recvarray+0) == userName) && (*(recvarray+1) == "LO")){
					sleep(1);
					send(deleteMessageByPosition);
					msg = "Exit from system have completed\x1A"; 									// String creation for sending
					sleep(1);
					send(telNumber);
					sleep(1);
					send(msg);
					sleep(1);
					cout << msg << endl;
					k1 = false;																		// Status False means end of current cycle
				}
				// Enabling of tracking mode
				if ((*(recvarray+0) == userName) && (*(recvarray+1) == "T")){						// if the 1st word of SMS is equal to username & 2nd word (command) is equal to "T" (tracking mode)
					sleep(1);
					send(deleteMessageByPosition);
					sleep(1);
					//Conversion of strings with temperature range to double type
					string tmin = *(recvarray+2);
					string tmax = *(recvarray+3);
					double tMin = atof(tmin.c_str());												// Convert 3rd word of SMS to tMin as double
					double tMax = atof(tmax.c_str());												// Convert 4th word of SMS to tMax as double
					cout << tmin << " - t min from pointer endl" << endl;
					cout << tmin << " - t man from pointer"<< endl;
					cout << tMin << " - t Min double"<< endl;
					cout << tMax << " - t Max double"<< endl;

					k2 = true;																		// Status true allows execution of tracking cycle
					while (k2 == true){
						cout << "tracking cycle" << endl;
						ncheck = false;
						// Reading and splitting of SMS (described above)
						send(msgTxtMode);
						recvarray = send(readMsg);
						T = T_ADC_reading();
						cout << T << " - Current T" << endl;
						word1 = *(recvarray+0);
						sleep(1);
						recvarray = send(readMsg);
						word1check = *(recvarray+0);
						lenOfString = strlen(word1.c_str());

						if (word1check == word1) {
						ncheck = true;
						}

						if (T > 50){
							gpioArg = "0";											// Low level of voltage on GPIO pin
							ofstream adc_on3 ("/sys/class/gpio/gpio48/value");		// Opening of GPIO value file
							adc_on3 << gpioArg;										// Write command to file
							adc_on3.close();
							sleep(30);
							gpioArg = "1";											// High level of voltage on GPIO pin
							ofstream adc_on4 ("/sys/class/gpio/gpio48/value");		// Opening of GPIO value file
							adc_on4 << gpioArg;										// Write command to file
							adc_on4.close();
							counter0 ++;
							if (counter0 > 4) {
								system ("init0");
							}
						}

						// Check on temperature conformity condition
						if ((T < tMin) || (T > tMax)){
							sleep(1);
							send(deleteMessageByPosition);
							sleep(1);
							strT = convTtoMsg(T);
							msg = "Warning! Current temperature:  " + strT + ctrlD; 				// String creation (Ambient temperature included)

							send(telNumber);
							sleep(1);
							send(msg);
							sleep(1);
							cout << "Temperature sent by T command" << endl;
							k2 = false;																// Status "k = false" means end of tracking cycle
						}
						// Check of current temperature in tracking mode
						if ((*(recvarray+0) == userName) && (*(recvarray+1) == "status")){
							sleep(1);
							send(deleteMessageByPosition);
							sleep(1);
							double T = T_ADC_reading();
							cout << T << endl;
							cout << telNumber << endl;
							strT = convTtoMsg(T);
							msg = "Current temperature: " + strT + ctrlD;
							sleep(1);
							send(telNumber);
							sleep(1);
							send(msg);
							sleep(1);
							cout << "Temperature sent by status2" << endl;
						}
						//End of cycle without SMS deleting in case of new boundaries
						if ((*(recvarray+0) == userName) && (*(recvarray+1) == "T")){
							k2 = false;
						}
						//End of cycle
						if ((*(recvarray+0) == userName) && (*(recvarray+1) == "ET")){
							sleep(1);
							send(deleteMessageByPosition);
							sleep(1);
							msg = "Tracking mode have turned off\x1A";
							send(telNumber);
							sleep(1);
							send(msg);
							sleep(1);
							k2 = false;
						}
						// Log out from system
						if ((*(recvarray+0) == userName) && (*(recvarray+1) == "LO")){
							sleep(1);
							send(deleteMessageByPosition);
							sleep(1);
							msg = "Exit from system have completed\x1A";
							send(telNumber);
							sleep(1);
							send(msg);			// Send warning message to user
							sleep(1);
							k2 = false;
							k1 = false;
						}
						if ((*(recvarray+0) != userName) && (lenOfString != 0) && (ncheck = true) ||
							((*(recvarray+1) != "T") && (*(recvarray+1) != "status") && (*(recvarray+1) != "LO") && (*(recvarray+1) != "ET")) && (lenOfString != 0) && (ncheck = true)){
							sleep(1);
							send(deleteMessageByPosition);
							sleep(1);
							telNumber = "AT+CMGS=" + *(recvarray+4) + bEnter;
							send(telNumber);
							sleep(1);
							send(msgFail);											// Send warning message to user
							sleep(1);
						}

					}

				}
			}
		}
	sleep(1);
	}
return 0;
}



