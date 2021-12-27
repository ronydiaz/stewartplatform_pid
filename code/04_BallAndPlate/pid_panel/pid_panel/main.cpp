/*
 * pid_panel.cpp
 *
 * Created: 20/12/2020 03:19:51 a. m.
 * Author : rony_
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "librerias/ports.h"
#include "librerias/servos.h"
#include "librerias/cinematica.h"
#include "librerias/mpu6050.h"
#include "librerias/uart.h"
#include "librerias/adc.h"
#include "librerias/pid.h"
#include "librerias/millis.h"

void servo_cero(){
	servo1(0);
	servo2(0);
	servo3(0);
	servo4(0);
	servo5(0);
	servo6(0);
}
void demo1(){
	cinematica_inversa(0,0,0,0,0,0);
	_delay_ms(1000);
	cinematica_inversa(3,0,0,0,0,0);
	_delay_ms(1000);
	
	cinematica_inversa(0,0,0,0,0,0);
	_delay_ms(1000);
	cinematica_inversa(0,3,0,0,0,0);
	_delay_ms(1000);
	
	cinematica_inversa(0,0,0,0,0,0);
	_delay_ms(1000);
	cinematica_inversa(0,0,1.5,0,0,0);
	_delay_ms(1000);
}

void demo_panel(){
	cinematica_inversa(0,0,0,u_10[0],u_9[0],0);
}

int main(void)
{
	cli();
	ports_init();
	uart_init();
	millis_init();
	servos_init();
	servos_start();
	adc_init();
	pid_init();
	sei();
	_delay_ms(1000);
	while (1)
	{
		demo_panel();
 		dtostrf(u_9[0],0,4,pid_1);
 		dtostrf(u_10[0],0,4,pid_2);
		
		uart_write("Posicion X: ");
		uart_write(set_X);
		uart_write("\tPID X: ");
		uart_write(pid_1);
		
		uart_write("\t\t\tPosicion Y: ");
		uart_write(set_Y);
		uart_write("\tPID Y: ");
		uart_write(pid_2);
		uart_write("\r\n");
		//_delay_ms(100);
	}
}