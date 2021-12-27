/*
 * millis.h
 *
 * Created: 01/12/2020 11:16:21 p. m.
 *  Author: rony_
 */ 


#ifndef MILLIS_H_
#define MILLIS_H_

#include <util/atomic.h>
//NOTA: Una variable de tipo unsigned long tiene valores de 0 to 4,294,967,295 (2^32 - 1). Cuando se desborda se reinicia a 0
unsigned long millis_counter;

ISR(TIMER0_OVF_vect){
	PORTJ ^= (1<<PJ5);
	millis_counter++;		// Aumenta el contador del millis
	TCNT0 = 6;				// Se reinicia el TCNT0
}

void millis_init()
{
	DDRJ |= (1<<DDJ5);
	TCCR0B &=~ (1<<CS02);	// Se selecciona un el prescalador 64
	TCCR0B |= (1<<CS01);
	TCCR0B |= (1<<CS00);
	TCNT0 = 6;				// Se hace el calculo para reiniciar el TCNT0 cada 1ms
	TIMSK0 |= (1<<TOIE0);	// Habilita la interrupcion por desbordamiento
}

unsigned long millis(){					//Esta funcion encapsula el valor del contador para evitar ser modificado o alterado por interrupciones
	unsigned long millis_return;
	ATOMIC_BLOCK(ATOMIC_FORCEON) {
		millis_return = millis_counter;
		}
	return millis_return;
}

#endif /* MILLIS_H_ */