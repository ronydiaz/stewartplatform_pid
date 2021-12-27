/*
 * adc.h
 *
 * Created: 22/11/2020 12:19:07 a. m.
 *  Author: rony_
 */ 


#ifndef ADC_H_
#define ADC_H_

void setup_ADC(){
	
	ADMUX &=~ (1<<ADLAR);	// Se desactiva el ajuste a la izquierda para leer los 10bits del ADC (H,L)
	ADMUX &=~ (1<<REFS1);	// Se selecciona el voltaje de referencia AVCC (5V)
	ADMUX |= (1<<REFS0);
	
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);		//Selecciona el prescalador de 128 para una frecuencia de muestreo entre 50kHz y 200kHz
	ADCSRA |= (1<<ADEN);							// Se habilita el ADC
	ADCSRA &=~ (1<<ADATE);							// Se deshabilita el autotrigger
	ADCSRA &=~ (1<<ADIE);							// Se deshabilitan las interrupciones del ADC
	
	ADCSRB &=~ (1<<ACME);							//No se utiliza el multiplexor por lo tanto se deshabilita
	ADCSRB &=~ (1<<ADTS2)&(1<<ADTS1)&(1<<ADTS0);	//Selecciona la fuente del autotrigger por defecto en Free Running

	DIDR0 |= 0x00;	//DIDR0 y DIDR2 deshabilitan las entradas digitales, como no se requiere se deshabilitan
	DIDR2 |= 0x00;
	
	// ----- Se realiza una primera conversion que normalmente es inestable para evitar ruido
	ADCSRA |= (1<<ADSC);			//Inicia la primera conversion
	while(!(ADCSRA & (1<<ADIF))){	//Pregunta por el estado de la bandera
	}
	ADCSRA |= (1<<ADIF);			//Se limpia la bandera
	//ADCSRA |= (1<<ADSC);			//Inicia la segunda conversion
}

uint16_t read_ADC(uint8_t canal)
{
	ADMUX &= 0b11100000;	// Limpia para evitar que se modifiquen los ultimos 3 bits de configuracion ADMUX
	ADCSRB &= 0b11110111;	// Limpia para evitar que se modifiquen los bits de configuracion ADCSRB a excepcion de MUX5

	ADMUX |= canal & 0x07;			// Selecciona el canal al registro ADMUX y previene que se modifique el MUX3 en caso de canal>=8
	ADCSRB = canal & (1<<MUX5);		//Set MUX5
	
	
	ADCSRA |= (1<<ADSC);			// Inicia la conversión
	while(!(ADCSRA & (1<<ADIF))){	// Espera hasta que a conversión este completa
	}
	ADCSRA |= (1<<ADIF);			// Se limpia la bandera
	
	return(ADC);				// Regresa el valor del ADC
}

#endif /* ADC_H_ */