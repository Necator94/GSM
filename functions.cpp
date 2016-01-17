#include <stdio.h>
#include <iostream>
#include "serialib.h"
#include "functions.h"
#include <stdlib.h>
#include <iostream>

#define DEVICE_PORT "/dev/ttyO4" 	// Path to serial port on BeagleBone Black

//Sending commands to device
string send(string command)
{
	serialib LS;
		const char * comm = command.c_str();//conversion of input argument to char * type which is nessecary for seriallib
		int Ret;
		char Buffer[128];					// Define size of buffer (amount of symbols in SMS)
		Ret = LS.Open(DEVICE_PORT, 9600);   // Open serial link at 9600 bauds
		Ret = LS.WriteString(comm);         // Send the command on the serial port
		string chek_reading_a(comm);		// Conversion read value to string type

		string chek_reading_b = "AT+CMGR=3\n";  		// Reading SMS string for next condition
		cout << "stratsss" << endl;
		sleep(1);
		if (chek_reading_a == chek_reading_b){  		// Check for command on presence of reading sms command
			// Read a maximum of 128 characters with a timeout of 5 seconds
			// The final character of the string must be a line feed ('\n')
			int i = 0;
			string smsLineArray[3];
			cout << "inside" << endl;
			while (i < 3) {
				Ret = LS.ReadString(Buffer,'\n',128,5000);	// Read first string from GSM module answer (sent command)
				sleep(0.1);
				string buf(Buffer);
				smsLineArray[i] = buf;
				i++;
			}
			cout << smsLineArray[1] << endl;

			istringstream ss(smsLineArray[1]);
			string token;
			i = 0;
			string strContent1[4];
			while(getline(ss, token, ',')) {
			    strContent1[i] = token;

			    i++;
			}
			cout << smsLineArray[2] << endl;
			istringstream ss1(smsLineArray[2]);
			string token1;
			i = 0;
			string strContent2[8];
			while(getline(ss1, token1, ' ')) {
				strContent2[i] = token1;

			    i++;
			    cout << strContent2[i] << "intokens" << endl;
			}


		return strContent2[0], strContent2[1], strContent2[2], strContent2[3], strContent1[1];
	}
   LS.Close();											// Close the connection with the device
   return string();
}
/*
//Splitting of strings on words
string splitstr(string *answ, string* w1, string* w2, string* w3)
{
	 	string buf; 					// Have a buffer string
	    stringstream ss(*answ); 		// Insert the string into a stream
	    vector <string> tokens; 		// Create vector to hold our words
	    //while (ss >> buf)
	    ss >> buf;
	    tokens.push_back(buf);
	    *answ = buf;					// Write first word to *answ
	    ss >> buf;
	    tokens.push_back(buf);
	    *w1 = buf;						// Write second word to *w1
	    ss >> buf;
	    tokens.push_back(buf);
	    *w2 = buf;						// Write third word to *w2
	    ss >> buf;
	    tokens.push_back(buf);
	    *w3 = buf;						// Write fourth word to *w3
        return string();
}
*/
//Check on existence of file (uses for checking ADC and UART)
bool FileExists(const char *fname)
		{
		return ifstream(fname) != NULL;
		}

//ADC and serial port enabling
 void ADC_UART_enabling()
{
	 const char *fnameADC = "/sys/devices/ocp.3/helper.15";    	// Path to ADC directory for existing check
	 const char *fnameUART = "/dev/ttyO4";						// Path to UART directory for existing check
	 if (FileExists(fnameADC) == 0){							// Enable ADC in case if it was not enabled earlier
		 string adcarg = "cape-bone-iio";						// Enabling command
		 ofstream adc_on ("/sys/devices/bone_capemgr.9/slots"); // Path to configuration directory for ADC
		 adc_on << adcarg;										// Write command to file
		 adc_on.close();
		 cout << "ADC_enabled" << endl;
	}
	 if (FileExists(fnameUART) == 0){							// Enable UART in case if it was not enabled earlier
		 string UARTarg = "BB-UART4";							// Enabling command
		 ofstream UART_on ("/sys/devices/bone_capemgr.9/slots");// Path to configuration directory for ADC
		 UART_on << UARTarg;									// Write command to file
		 UART_on.close();
		 cout << "UART_enabled" << endl;
	 }
}

 //Reading and processing of ADC value to temperature
double T_ADC_reading()
{
	// ifstream adc_val1 ("/sys/devices/ocp.3/helper.15/AIN0");				// Path to file which contains value of ADC(0-1800)
	int n = 0;
	double T = 0;
	while (n < 1000){
		ifstream adc_val ("/sys/bus/iio/devices/iio\:device0/in_voltage0_raw"); // Path to file which contains value of ADC(0-4096)
		double adc_value;
		adc_val >> adc_value;													// Write ADC value to variable
		adc_value = 1.8 / 4095 * adc_value;										// Calculation
		double Rt = (816 * adc_value) / (1.8 - adc_value); 				// Calculation of resulted resistance
		T = T + (Rt - 1000) * 0.25;
		n ++;
	}
	T = T / 1000;
	return T;											// Return resulted temperature as argument
}

