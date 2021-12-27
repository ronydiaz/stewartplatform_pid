/*
 * ports.h
 *
 * Created: 20/11/2020 07:15:10 p. m.
 *  Author: rony_
 */ 


#ifndef PORTS_H_
#define PORTS_H_

void ports_init(){
// ----- CONFIGURAR PUERTO A -----
// 	DDRA &=~ (1<<PA0); // Declarar entradas del PORTA
// 	DDRA |= (1<<PA1); // Declarar salidas del PORTA
// 	PORTA &=~ (1<<PA0); // Configura resistencia en PORTA0 pull-down
// 	PORTA |= (1<<PA0); // Configura resistencia en PORTA0 pull-up
// 	PORTA &=~ (1<<PA1); // Configura salida PORTA1 en estado bajo (0)
// 	PORTA |= (1<<PA1); // Configura salida PORTA1 en estado alto (1)

// ----- CONFIGURAR PUERTO B -----
// 	DDRB &=~ 1;
// 	DDRB |= 0;
// 	PORTB &=~ 1;	
// 	PORTB |= 0;

// ----- CONFIGURAR PUERTO C -----
// 	DDRC &=~ 1;
// 	DDRC |= 0;
// 	PORTC &=~ 1;
// 	PORTC |= 0;

// ----- CONFIGURAR PUERTO D -----
// 	DDRD &=~ 1;
// 	DDRD |= 0;
// 	PORTD &=~ 1;
// 	PORTD |= 0;

// ----- CONFIGURAR PUERTO E -----
// 	DDRE &=~ 1;
// 	DDRE |= 0;
// 	PORTE &=~ 1;
// 	PORTE |= 0; 	

// ----- CONFIGURAR PUERTO F -----
	DDRF &=~ (1<<DDF0); // Se declaran entradas del PORTF
	DDRF |= (1<<DDF1)|(1<<DDF2)|(1<<DDF3)|(1<<DDF4); // Se declaran salidas del PORT F
	PORTF &=~ (1<<PF0); // Se configura resistencia pull-down 
	PORTF = (0<<PF1)|(0<<PF2)|(0<<PF3)|(0<<PF4);

// ----- CONFIGURAR PUERTO G -----
// 	DDRG &=~ 1;
// 	DDRG |= 0;
// 	PORTG &=~ 1;
// 	PORTG |= 0;

// ----- CONFIGURAR PUERTO H -----
// 	DDRH &=~ 1;
// 	DDRH |= 0;
// 	PORTH &=~ 1;
// 	PORTH |= 0;

// ----- CONFIGURAR PUERTO J -----
// 	DDRJ &=~ 1;
// 	DDRJ |= 0;
// 	PORTJ &=~ 1;
// 	PORTJ |= 0;

// ----- CONFIGURAR PUERTO K -----
// 	DDRK &=~ 1;
// 	DDRK |= 0;
// 	PORTK &=~ 1;
// 	PORTK |= 0;

// ----- CONFIGURAR PUERTO L -----
// 	DDRL &=~ 1;
// 	DDRL |= 0;
// 	PORTL &=~ 1;
// 	PORTL |= 0;

}



#endif /* PORTS_H_ */