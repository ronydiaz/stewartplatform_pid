/*
 * servos.h
 *
 * Created: 28/11/2020 03:16:35 a. m.
 *  Author: rony_
 */ 


#ifndef SERVOS_H_
#define SERVOS_H_

#define prescalador 8 // Prescalador de la frecuencia
#define f_servos 50
#define TOP_timer ((F_CPU/prescalador/f_servos) - 1)

float pot[6]={};
float pot_giro[6]={};
char pot1[10],pot2[10],pot3[10],pot4[10],pot5[10],pot6[10];

void servos_init()
{
//	-	-	-	-	-	-	-	-	-	- T I M E R   3 -	-	-	-	-	-	-	-	-	-
	
	//	-	-	-	-	-	CONFIGURACION DEL REGISTRO A	-	-	-	-	-
	TCCR3A &=~ (1<<WGM30); // WGM3(3:0) Configura Fast PWM como modo de operacion del timer 3 (estan en registros A y B)
	TCCR3A |=  (1<<WGM31);
	TCCR3B |=  (1<<WGM32);
	TCCR3B |=  (1<<WGM33);
	//	-	-	-	-	-	CONFIGURACION DEL REGISTRO B	-	-	-	-	-
	TCCR3B &=~ (1<<ICNC3);	// Inhabilita el filtro para interrupciones externas como fuente del Timer 3

	TCCR3B &=~ (1<<CS32);	// CS3(2:0) Selecciona el timer3 sin fuente para iniciar apagado
	TCCR3B &=~ (1<<CS31);
	TCCR3B &=~ (1<<CS30);
	//	-	-	-	-	-	CONFIGURACION DEL REGISTRO C	-	-	-	-	-
	TCCR3C = 0x00;	// Inhabilita las comparaciones forzadas para OCnA, OCnB y OCnC
	TIMSK3 = 0x00;	// Se inhabilitan las interrupciones
	TIFR3 = 0x00;	// Se inhabilitan las banderas
	
	ICR3 = TOP_timer;	// Asigna el valor TOP del timer 3
//	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-
	
//	-	-	-	-	-	-	-	-	-	- T I M E R   4 -	-	-	-	-	-	-	-	-	-
	//	-	-	-	-	-	CONFIGURACION DEL REGISTRO A	-	-	-	-	-
	TCCR4A &=~ (1<<WGM40); // WGM4(3:0) Configura Fast PWM como modo de operacion del timer 4 (estan en registros A y B)
	TCCR4A |=  (1<<WGM41);
	TCCR4B |=  (1<<WGM42);
	TCCR4B |=  (1<<WGM43);
	//	-	-	-	-	-	CONFIGURACION DEL REGISTRO B	-	-	-	-	-
	TCCR4B &=~ (1<<ICNC4);	// Inhabilita el filtro para interrupciones externas como fuente del Timer 4
	TCCR4B &=~ (1<<CS42);	// CS4(2:0) Selecciona el timer4 sin fuente para iniciar apagado
	TCCR4B &=~ (1<<CS41);
	TCCR4B &=~ (1<<CS40);
	//	-	-	-	-	-	CONFIGURACION DEL REGISTRO C	-	-	-	-	-
	TCCR4C = 0x00;	// Inhabilita las comparaciones forzadas para OCnA, OCnB y OCnC
	//	-	-	-	-	-	CONFIGURACION DE INTERRUPCIONES Y TOP DEL TIMER4
	TIMSK4 = 0x00;	// Se inhabilitan las interrupciones
	TIFR4 = 0x00;	// Se inhabilitan las banderas
	ICR4 = TOP_timer; }	// Asigna el valor TOP del timer 4

void servos_start()
{
//	-	-	-	-	-	-	-	-	-	- T I M E R   3 -	-	-	-	-	-	-	-	-	-
	TCNT3 = 0x0000;			// Limpia el contador del timer 4
	TCCR3B &=~ (1<<CS32);	// Selecciona la fuente del timer3 con N = 8
	TCCR3B |= (1<<CS31);
	TCCR3B &=~ (1<<CS30);
//	-	-	-	-	-	-	-	-	-	- T I M E R   4 -	-	-	-	-	-	-	-	-	-
	TCNT4 = 0x0000;			// Limpia el contador del timer 4
	TCCR4B &=~ (1<<CS42);	// Selecciona la fuente del timer3 con N = 8
	TCCR4B |= (1<<CS41);
	TCCR4B &=~ (1<<CS40); }

void servos_stop()
{
//	-	-	-	-	-	-	-	-	-	- T I M E R   3 -	-	-	-	-	-	-	-	-	-
	TCCR3B &=~ (1<<CS32);	// Configura al timer3 sin fuente del reloj
	TCCR3B &=~ (1<<CS31);
	TCCR3B &=~ (1<<CS30);
	TCNT3 = 0x0000;			// Limpia el contador del timer 3
//	-	-	-	-	-	-	-	-	-	- T I M E R   4 -	-	-	-	-	-	-	-	-	-
	TCCR4B &=~ (1<<CS42);	// Configura al timer4 sin fuente del reloj
	TCCR4B &=~ (1<<CS41);
	TCCR4B &=~ (1<<CS40);
	TCNT4 = 0x0000;	}		// Limpia el contador del timer 4

void servo1(float posicion)
{
	if (posicion>80)											// Anti WindUP 
	{	posicion=80;}
	if (posicion<-80)
	{	posicion=-80;}
	float duty;
	duty = 2 + (((13-2)*(-posicion-(-90)))/(90-(-90)));		// Y = Y0 + (Y1-Y0)*(X-X0)/(X1-X0)		D = 13-2
	TCCR3A |=  (1<<COM3A1);				// Activa la salida del comparador OCR3A como no invertida para Fast PWM
	TCCR3A &=~ (1<<COM3A0);
	OCR3A = (TOP_timer*duty)/100;		// Asigna el valor del dutycicle
}
void servo2(float posicion)
{
	if (posicion>80)											// Anti WindUP
	{	posicion=80;}
	if (posicion<-80)
	{	posicion=-80;}
	float duty;
	duty = 2 + (((13-2)*(posicion-(-90)))/(90-(-90)));		// Y = Y0 + (Y1-Y0)*(X-X0)/(X1-X0)		D = 13-2
	TCCR3A |=  (1<<COM3B1);				// Activa la salida del comparador OCR3B como no invertida para Fast PWM
	TCCR3A &=~ (1<<COM3B0);
	OCR3B = (TOP_timer*duty)/100;		// Asigna el valor del dutycicle
}
void servo3(float posicion)
{
	if (posicion>80)											// Anti WindUP
	{	posicion=80;}
	if (posicion<-80)
	{	posicion=-80;}
	float duty;
	duty = 2 + (((13-2)*(-posicion-(-90)))/(90-(-90)));		// Y = Y0 + (Y1-Y0)*(X-X0)/(X1-X0)
	TCCR3A |=  (1<<COM3C1);				// Activa la salida del comparador OCR3C como no invertida para Fast PWM
	TCCR3A &=~ (1<<COM3C0);
	OCR3C = (TOP_timer*duty)/100;		// Asigna el valor del dutycicle
}
void servo4(float posicion)
{
	if (posicion>80)											// Anti WindUP
	{	posicion=80;}
	if (posicion<-80)
	{	posicion=-80;}
	float duty;
	duty = 2 + (((12-2)*(posicion-(-90)))/(90-(-90)));		// Y = Y0 + (Y1-Y0)*(X-X0)/(X1-X0)
	TCCR4A |=  (1<<COM4A1);				// Activa la salida del comparador OCR3A como no invertida para Fast PWM
	TCCR4A &=~ (1<<COM4A0);
	OCR4A = (TOP_timer*duty)/100;		// Asigna el valor del dutycicle
}
void servo5(float posicion)
{
	if (posicion>80)											// Anti WindUP
	{	posicion=80;}
	if (posicion<-80)
	{	posicion=-80;}
	float duty;
	duty = 3 + (((13-3)*(-posicion-(-90)))/(90-(-90)));		// Y = Y0 + (Y1-Y0)*(X-X0)/(X1-X0)
	TCCR4A |=  (1<<COM4B1);				// Activa la salida del comparador OCR3B como no invertida para Fast PWM
	TCCR4A &=~ (1<<COM4B0);
	OCR4B = (TOP_timer*duty)/100;		// Asigna el valor del dutycicle
}
void servo6(float posicion)
{
	if (posicion>80)											// Anti WindUP
	{	posicion=80;}
	if (posicion<-80)
	{	posicion=-80;}
	float duty;
	duty = 2 + (((12-2)*(posicion-(-90)))/(90-(-90)));		// Y = Y0 + (Y1-Y0)*(X-X0)/(X1-X0)
	TCCR4A |=  (1<<COM4C1);				// Activa la salida del comparador OCR3C como no invertida para Fast PWM
	TCCR4A &=~ (1<<COM4C0);
	OCR4C = (TOP_timer*duty)/100;		// Asigna el valor del dutycicle
}

#endif /* SERVOS_H_ */