/*
 * pid_giro.cpp
 *
 * Created: 19/12/2020 07:20:16 p. m.
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
	cinematica_inversa(4,0,0,0,0,0);
	_delay_ms(1000);
	cinematica_inversa(-4,0,0,0,0,0);
	_delay_ms(1000);
	
	cinematica_inversa(0,0,0,0,0,0);
	_delay_ms(1000);
	cinematica_inversa(0,4,0,0,0,0);
	_delay_ms(1000);
	cinematica_inversa(0,-4,0,0,0,0);
	_delay_ms(1000);
	
	cinematica_inversa(0,0,0,0,0,0);
	_delay_ms(1000);
	cinematica_inversa(0,0,1.5,0,0,0);
	_delay_ms(1000);
}
void demo_giro(){
	mpu6050_read();
	cinematica_inversa(0,0,0,-ang_x,-ang_y,0);
}
void demo_giro_pid(){
	mpu6050_read();
	cinematica_inversa(0,0,0,u_7[0],u_8[0],0);
}

int main(void)
{
	cli();
	ports_init();
	uart_init();
	i2c_init();
	mpu6050_init();
	servos_init();
	servos_start();
	adc_init();
	pid_init();
	sei();

	cinematica_inversa(0,0,0,0,0,0);
	_delay_ms(1000);
	while (1)
	{	
		demo_giro_pid();
		//servo_cero();
		uart_write("\tGiroX: ");
		uart_write(set_Gx);
		uart_write("\tPID ALFA: ");
		uart_write(pidalfa);
		uart_write("\t\t\tGiroY: ");
		uart_write(set_Gy);
		uart_write("\tPID BETA: ");
		uart_write(pidbeta);
		uart_write("\r\n");
	}
}
