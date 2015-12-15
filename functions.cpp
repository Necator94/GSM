#include <stdio.h>
#include <iostream>
#include "serialib.h"
#include "functions.h"


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

		string chek_reading_b = "AT+CMGR=1\n";  		// Reading SMS string for next condition
		if (chek_reading_a == chek_reading_b){  		// Check for command on presence of reading sms command
			// Read a maximum of 128 characters with a timeout of 5 seconds
			// The final character of the string must be a line feed ('\n')
			Ret = LS.ReadString(Buffer,'\n',128,5000);	// Read first string from GSM module answer (send command)
			Ret = LS.ReadString(Buffer,'\n',128,5000);	// Read second string from GSM module answer (GSM answer)
			Ret = LS.ReadString(Buffer,'\n',128,5000);	// Read third string from GSM module answer (SMS text)
			string str(Buffer);							// Write string to buffer
			return str;									// Return buffer as argument
		} else {  // This condition used for debugging
			Ret = LS.ReadString(Buffer,'\n',128,5000);
			Ret = LS.ReadString(Buffer,'\n',128,5000);	// Read an answer of GSM module
			string str1(Buffer);
			return str1;
		}
   LS.Close();											// Close the connection with the device
return string();
}

//Splitting of strings on words
string fun(string *answ, string* w1, string* w2, string* w3)
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
	 ifstream adc_val ("/sys/bus/iio/devices/iio\:device0/in_voltage0_raw"); // Path to file which contains value of ADC(0-4096)
	 double adc_value;
	 adc_val >> adc_value;								// Write ADC value to variable
	 adc_value = 1.8 / 4095 * adc_value;				// Calculation
	// cout << adc_value << " - 4096" << endl;
	 double Rt = (800 * adc_value) / (1.8 - adc_value); // Calculation of resulted resistance
	 double T = (Rt - 1000) * 0.25;						// Calculation of resulted temperature
	// cout << T << " - 4096" << endl;
	 return T;											// Return resulted temperature as argument
}

