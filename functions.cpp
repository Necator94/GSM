#include <stdio.h>
#include <iostream>
#include "serialib.h"
#include "functions.h"


#define DEVICE_PORT "/dev/ttyO4"
																									// Open serial port

string send(string command)
{
	serialib LS;

		const char * comm = command.c_str();
		int Ret;
		char Buffer[128];
		Ret=LS.Open(DEVICE_PORT,9600);                                        // Open serial link at 115200 bauds
		Ret=LS.WriteString(comm);                                            // Send the command on the serial port
		string chek_reading_a(comm);											// Read a second string from the serial device
		string chek_reading_b="AT+CMGR=1\n";

		if (chek_reading_a==chek_reading_b)
		{
			Ret=LS.ReadString(Buffer,'\n',128,5000);   								// Read a maximum of 128 characters with a timeout of 5 seconds
			Ret=LS.ReadString(Buffer,'\n',128,5000);
			Ret=LS.ReadString(Buffer,'\n',128,5000);					// The final character of the string must be a line feed ('\n')
			string str(Buffer);                                                      // If a string has been read from, print the string
		return str;
		}
			else
			{
				Ret=LS.ReadString(Buffer,'\n',128,5000);
				Ret=LS.ReadString(Buffer,'\n',128,5000);
				string str1(Buffer);
			return str1;
			}                                                            // If a string has been read from, print the string

   LS.Close();															// Close the connection with the device
return string();
}
//*********************************separation
string fun(string *answ, string* w1, string* w2)
{
	 	string buf; 								// Have a buffer string
	    stringstream ss(*answ); 						// Insert the string into a stream
	    vector<string> tokens; 						// Create vector to hold our words
	    //while (ss >> buf){
	    ss>>buf;
	    tokens.push_back(buf);
	    *answ=buf;
	    ss>>buf;
	    tokens.push_back(buf);
	    *w1=buf;
	    ss>>buf;
	    tokens.push_back(buf);
	    *w2=buf;
        //i++;
	    //}
return string();
}
//*********************************end of separation
bool FileExists(const char *fname)
		 			{
		 			  return std::ifstream(fname) != NULL;
		 			}
//***********************ADC enabling****************
 void adc_UART_enabling()
{
	 const char *fnameADC="/sys/devices/ocp.3/helper.15";
	 bool a=FileExists(fnameADC);
	 if (a==0)
	{
	 cout<<a<<endl;
	}
	 else {
		 cout <<"kek"<<a<<endl;

	 }
/*
string adcarg="cape-bone-iio";
ofstream adc_on ("/sys/devices/bone_capemgr.9/slots");
adc_on<<adcarg;
adc_on.close();
*/
/*string UARTarg="BB-UART4";
ofstream UART_on ("/sys/devices/bone_capemgr.9/slots");
UART_on<<UARTarg;
UART_on.close();
*/}
//***************************************************

