/*
 * ControlServos.cpp
 *
 * Created: 11/12/2020 03:39:26 a. m.
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
	
	//int x[6]={0,0,0,0,0,0};
	float pot[6]={};
	float pot_giro[6]={};
	char pot1[10],pot2[10],pot3[10],pot4[10],pot5[10],pot6[10];
	/*cinematica_inversa(0,0,0,0,0,0);*/
	_delay_ms(1000);
    while (1) 
    {
		mpu6050_read();
		cinematica_inversa(0,0,0,-ang_x,-ang_y,0);
		pot[0] = read_ADC(8);
		pot_giro[0] = -80 + (((80-(-80))*(pot[0]-(1)))/(464-(0)));
		
		pot[1] = read_ADC(9);
		pot_giro[1] = -80 + (((80-(-80))*(pot[1]-(0)))/(592-(0)));
		
		pot[2] = read_ADC(10);
		pot_giro[2] = -80 + (((80-(-80))*(pot[2]-(0)))/(514-(0)));
		
		pot[3] = read_ADC(11);
		pot_giro[3] = -80 + (((80-(-80))*(pot[3]-(0)))/(622-(0)));
		
		pot[4] = read_ADC(12);
		pot_giro[4] = -80 + (((80-(-80))*(pot[4]-(0)))/(546-(0)));
		
		pot[5] = read_ADC(13);
		pot_giro[5] = -80 + (((80-(-80))*(pot[5]-(0)))/(568-(0)));
		
		dtostrf(pot_giro[0],0,4,pot1);
		dtostrf(pot_giro[1],0,4,pot2);
		dtostrf(pot_giro[2],0,4,pot3);
		dtostrf(pot_giro[3],0,4,pot4);
		dtostrf(pot_giro[4],0,4,pot5);
		dtostrf(pot_giro[5],0,4,pot6);
		uart_write("Servo1: ");
		uart_write(pot1);
		uart_write("\t\tServo2: ");
		uart_write(pot2);
		uart_write("\t\tServo3: ");
		uart_write(pot3);
		uart_write("\t\tServo4: ");
		uart_write(pot4);
		uart_write("\t\tServo5: ");
		uart_write(pot5);
		uart_write("\t\tServo6: ");
		uart_write(pot6);
		uart_write("\t\tGx: ");
		uart_write(set_Gx);
		uart_write("\t\tGy: ");
		uart_write(set_Gy);
		uart_write("\n\r");
		//_delay_ms(100);
		
    }
}

