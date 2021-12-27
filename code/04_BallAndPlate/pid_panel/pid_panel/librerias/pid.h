/*
 * pid.h
 *
 * Created: 17/12/2020 07:17:21 p. m.
 *  Author: rony_
 */ 


#ifndef PID_H_
#define PID_H_

#include <avr/interrupt.h>
#include "adc.h"
#include "cinematica.h"
#include "servos.h"
#include "panel.h"

#define T 0.010				//Se define el tiempo de muestreo en ms
float kp=8;
float ki=0;
float kd=-1;

float c_2 = kd;
float c_1 = ki*T - kp - 2*kd;
float c_0 = kp + kd;
char pid_1[8],pid_2[8],pid_3[8],pid_4[8],pid_5[8],pid_6[8];

//	-	-	-	-	-	-	-	-	-	-	-	-	-	Variables P I D Servomotor 1	-	-	-	-	-	-	-	-	-	-	-	//
float set_panelX;
float e_9[3]={0.0,0.0,0.0};				
float u_9[2]={0.0,0.0};
	
//	-	-	-	-	-	-	-	-	-	-	-	-	-	Variables P I D Servomotor 2	-	-	-	-	-	-	-	-	-	-	-	//
float set_panelY;
float e_10[3]={0.0,0.0,0.0};
float u_10[2]={0.0,0.0};

ISR(TIMER1_COMPA_vect){
	PORTJ ^= (1<<PJ6);
	read_PR();
	////////////////////////////////////////////////////////// P I D Panel Resistivo X /////////////////////////////////////////////////
	set_panelX = 0;											// Se define el valor de referencia deseado, aqui
	
	e_9[0]=set_panelX-pX;									// Se define el error como el valor deseado menos la retroalimentacion
	
	u_9[0]=c_0*e_9[0]+c_1*e_9[1]+c_2*e_9[2]+u_9[1];			// Calcular PID
	
	if (u_9[0]>5)											// Anti WindUP para los valores del PID
	{	u_9[0]=5;}
	if (u_9[0]<-5)
	{	u_9[0]=-5;}	

	e_9[2]=e_9[1];											// Realizar los corrimientos de salidas y errores
	e_9[1]=e_9[0];
	u_9[1]=u_9[0];
	
	////////////////////////////////////////////////////////// P I D Panel Resisitivo Y /////////////////////////////////////////////////
	set_panelY = 0;											// Se define el valor de referencia deseado, aqui
	
	e_10[0]=set_panelY-(pY);									// Se define el error como el valor deseado menos la retroalimentacion
	
	u_10[0]=c_0*e_10[0]+c_1*e_10[1]+c_2*e_10[2]+u_10[1];	// Calcular PID
	
	if (u_10[0]>5)											// Anti WindUP para los valores del PID
	{	u_10[0]=5;}
	if (u_10[0]<-5)
	{	u_10[0]=-5;}

	e_10[2]=e_10[1];											// Realizar los corrimientos de salidas y errores
	e_10[1]=e_10[0];
	u_10[1]=u_10[0];
}

void pid_init()
{
	DDRJ |= (1<<DDJ6);
	
	TCCR1B &=~ (1<<WGM13);	// Selecciona el modo CTC por OCRA
	TCCR1B |= (1<<WGM12);
	TCCR1A &=~ (1<<WGM11);
	TCCR1A &=~ (1<<WGM10);
	
	TCCR1B &=~ (1<<CS12);	// Se selecciona un el prescalador 64 para tener un tiempo maximo
	TCCR1B |= (1<<CS11);
	TCCR1B |= (1<<CS10);
	
	OCR1A = (F_CPU/64/(1/T)) - 1;		// Se hace el calculo para reiniciar el TCNT0 cada 1ms
	TIMSK1 |= (1<<OCIE1A);				// Habilita la interrupcion por desbordamiento
}
void pid_on(){
	TCNT1 = 0x0000;
	TCCR1B &=~ (1<<CS12);	// Se selecciona un el prescalador 64
	TCCR1B |= (1<<CS11);
	TCCR1B |= (1<<CS10);
}
void pid_off(){
	TCCR1B &=~ (1<<CS10);
	TCCR1B &=~ (1<<CS11);
	TCCR1B &=~ (1<<CS12);
	TCNT1 = 0x0000;
}


#endif /* PID_H_ */