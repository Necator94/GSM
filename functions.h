/*
 * functions.h
 *
 *  Created on: 19 нояб. 2015 г.
 *      Author: ivan
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
#include <string>
#include <cstring>
#include <vector>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include "serialib.h"
#include <stdlib.h>
#include <iostream>
#include "unistd.h"
using namespace std;


string * send(string);									// Function for sending of commands to GSM module
string splitstr(string*, string*, string*, string*);			// Function for splitting of SMS on variables
void ADC_UART_enabling();								// Function for enabling of ADC and UART port
bool FileExists(const char*);							// Function for checking on existence of files
double T_ADC_reading();									// Function for reading of ADC value
string convTtoMsg(double);
#endif /* FUNCTIONS_H_ */
