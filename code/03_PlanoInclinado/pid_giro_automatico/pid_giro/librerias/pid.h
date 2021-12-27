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
#include "mpu6050.h"
#include "cinematica.h"
#include "servos.h"

#define T 0.010				//Se define el tiempo de muestreo en ms
float kp=0.0586;
float ki=3;
float kd=0.8;
float c_2 = kd;
float c_1 = ki*T - kp - 2*kd;
float c_0 = kp + kd;
char pid_1[8],pid_2[8],pid_3[8],pid_4[8],pid_5[8],pid_6[8],pidalfa[8],pidbeta[8];

//	-	-	-	-	-	-	-	-	-	-	-	-	-	Variables P I D Servomotor 1	-	-	-	-	-	-	-	-	-	-	-	//
float set_servo1, A1, retro1;
float e_1[3]={0.0,0.0,0.0};				
float u_1[2]={0.0,0.0};
	
//	-	-	-	-	-	-	-	-	-	-	-	-	-	Variables P I D Servomotor 2	-	-	-	-	-	-	-	-	-	-	-	//
float set_servo2, A2, retro2;
float e_2[3]={0.0,0.0,0.0};
float u_2[2]={0.0,0.0};
	
//	-	-	-	-	-	-	-	-	-	-	-	-	-	Variables P I D Servomotor 3	-	-	-	-	-	-	-	-	-	-	-	//
float set_servo3, A3, retro3;
float e_3[3]={0.0,0.0,0.0};
float u_3[2]={0.0,0.0};
	
//	-	-	-	-	-	-	-	-	-	-	-	-	-	Variables P I D Servomotor 4	-	-	-	-	-	-	-	-	-	-	-	//
float set_servo4, A4, retro4;
float e_4[3]={0.0,0.0,0.0};
float u_4[2]={0.0,0.0};
	
//	-	-	-	-	-	-	-	-	-	-	-	-	-	Variables P I D Servomotor 5	-	-	-	-	-	-	-	-	-	-	-	//
float set_servo5, A5, retro5;
float e_5[3]={0.0,0.0,0.0};
float u_5[2]={0.0,0.0};
	
//	-	-	-	-	-	-	-	-	-	-	-	-	-	Variables P I D Servomotor 6	-	-	-	-	-	-	-	-	-	-	-	//
float set_servo6, A6, retro6;
float e_6[3]={0.0,0.0,0.0};
float u_6[2]={0.0,0.0};

//	-	-	-	-	-	-	-	-	-	-	-	-	-	Variables P I D Giro X	-	-	-	-	-	-	-	-	-	-	-	//
float set_alpha;
float e_7[3]={0.0,0.0,0.0};
float u_7[2]={0.0,0.0};

//	-	-	-	-	-	-	-	-	-	-	-	-	-	Variables P I D Giro Y	-	-	-	-	-	-	-	-	-	-	-	//
float set_beta;
float e_8[3]={0.0,0.0,0.0};
float u_8[2]={0.0,0.0};

ISR(TIMER1_COMPA_vect){
	PORTJ ^= (1<<PJ6);
	
	////////////////////////////////////////////////////////// P I D para beta /////////////////////////////////////////////////
	set_alpha = 0;											// Se define el valor de referencia deseado, aqui
	
	e_7[0]= set_alpha-ang_x;								// Se define el error como el valor deseado menos la retroalimentacion
	
	u_7[0]=c_0*e_7[0]+c_1*e_7[1]+c_2*e_7[2]+u_7[1];			// Calcular PID
	dtostrf(u_7[0],2,2,pidalfa);
	
	if (u_7[0]>12)											// Anti WindUP para los valores del PID
	{	u_7[0]=12;}
	if (u_7[0]<-12)
	{	u_7[0]=-12;}

	e_7[2]=e_7[1];											// Realizar los corrimientos de salidas y errores
	e_7[1]=e_7[0];
	u_7[1]=u_7[0];
	
	////////////////////////////////////////////////////////// P I D para alpha /////////////////////////////////////////////////
	set_beta = 0;											// Se define el valor de referencia deseado, aqui
	
	e_8[0]= set_beta-ang_y;								// Se define el error como el valor deseado menos la retroalimentacion
	
	u_8[0]=c_0*e_8[0]+c_1*e_8[1]+c_2*e_8[2]+u_8[1];			// Calcular PID
	dtostrf(u_8[0],2,2,pidbeta);
	
	if (u_8[0]>12)											// Anti WindUP para los valores del PID
	{	u_8[0]=12;}
	if (u_8[0]<-12)
	{	u_8[0]=-12;}

	e_8[2]=e_8[1];											// Realizar los corrimientos de salidas y errores
	e_8[1]=e_8[0];
	u_8[1]=u_8[0];
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