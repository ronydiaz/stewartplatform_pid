/*
 * LazoAbierto.cpp
 *
 * Created: 19/12/2020 04:34:44 p. m.
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

void demo2(){
	cinematica_inversa(0,0,0,0,0,0);
	_delay_ms(500);
	cinematica_inversa(2,0,0,0,-10,0);
	_delay_ms(500);
	
	cinematica_inversa(0,0,0,0,0,0);
	_delay_ms(500);
	cinematica_inversa(-2,0,0,0,10,0);
	_delay_ms(500);
}

void demo_giro(){
	read_MPU6050();
	cinematica_inversa(0,0,0,ang_x,ang_y,0);
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
	sei();

	cinematica_inversa(0,0,0,0,0,0);
	_delay_ms(1000);
	while (1)
	{
		demo_giro();
		
	}
}