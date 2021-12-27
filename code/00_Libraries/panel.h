/*
 * panel.h
 *
 * Created: 17/12/2020 01:47:39 a. m.
 *  Author: rony_
 */ 


#ifndef PANEL_H_
#define PANEL_H_

#include "adc.h"
#include "ports.h"
#include "millis.h"

unsigned long panel_time;
#define asentamiento 1
char estado;
int temp;
float panel_aX, panel_aY, pX, pY;
char set_X[5], set_Y[5];

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////// PROCESO DE ASENTAMIENTO /////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define U_LONG_MAX 4294967295
#define U_INT_MAX 4294967295

unsigned long loop_tiempo(unsigned long s,unsigned long e,int t)
{
	unsigned long ret;
	// Is our start after our end?
	if (s > e)
	{ // It is, so we'll need to do some jig-higgery
		// Let's get our difference
		ret = s-e;
		// What type of variable are we using?
		if (t)
		{ // Unsigned long
			// Correct with our U_LONG_MAX
			ret = U_LONG_MAX - ret;
		} else
		{ // Unsigned int
			// Correct with our U_INT_MAX
			ret = U_INT_MAX - ret;
		}
	} else
	{ // It isn't so we can just subtract
		ret = e-s;
	}
	// Return our difference
	return ret;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////// PROCESO DE ASENTAMIENTO /////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void read_PR(){
	switch(estado)
	{
		case 0: // Set up X read
		PORTF = 0b00001100;
		panel_time = millis();
		estado = 1;
		break;
		
		case 1:
		temp = loop_tiempo(panel_time, millis(), true);
		if (temp >= asentamiento)
		{
		estado = 2;}
		break;

		case 2: // Read the X position
		panel_aX=read_ADC(0);      //Lee el valor del ADC
		estado = 3;
		break;
		
		case 3: // Set up Y read
		PORTF = 0b00000110;
		panel_time = millis();
		estado = 4;
		break;
		
		case 4:
		temp = loop_tiempo(panel_time, millis(), true);
		if (temp >= asentamiento)
		{
		estado = 5;}
		break;

		case 5:
		panel_aY=read_ADC(0);      //Lee el valor del ADC de PC0
		estado = 6;
		break;

		case 6:
		pX = -161 + (161-(-161))*(panel_aX-250)/(750-250);
		//X2 = 0 + (100-0)*(X-(-32))/(32-(-32));
		pY = -124 + (124-(-124))*(panel_aY-250)/(750-250);
		//Y2 = 0 + (100-0)*(Y-(-25))/(25-(-25));
		dtostrf(pX, 0, 0, set_X);
		dtostrf(pY, 0, 0, set_Y);
		estado = 7;
		break;

		case 7:
		PORTF = 0b00000000;
		estado = 0;
		break;
		
		default: // Error
		uart_write("\n\rAlgo anda mal bro");
		break;
	}
}

#endif /* PANEL_H_ */