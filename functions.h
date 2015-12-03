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
using namespace std;


string send(string);
string fun(string*, string*, string*, string*);
void ADC_UART_enabling();
bool FileExists(const char*);
float T_ADC_reading();




#endif /* FUNCTIONS_H_ */
