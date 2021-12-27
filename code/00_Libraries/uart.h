/*
 * uart.h
 *
 * Created: 26/11/2020 01:26:08 a. m.
 *  Author: rony_
 */ 


#ifndef UART_H_
#define UART_H_

#define DEBUG 1
#define BAUDRATE 9600
#define BAUDRATE_UBRR0 ((F_CPU/8/BAUDRATE)-1)

void uart_init(){
	//	-	-	-	-	-	Configuracion del Registro A	-	-	-	-	-
	UCSR0A &=~ (1<<MPCM0);
	//	MPCM0 Deshabilita el multiprecesador
	UCSR0A |= (1<<U2X0);
	//	U2X0 Habilita la velocidad doble de transmision ya que se usara el modo asincrono del USART
	
	//	-	-	-	-	-	Configuracion del Registro B	-	-	-	-	-
	UCSR0B &=~ (1<<TXCIE0)&(1<<UDRIE0)&(1<<UCSZ02)&(1<<RXB80)&(1<<TXB80);
	//	TXCIE0 Deshabilita las interrupciones por transmision de datos
	//	UDRIE0 Deshabilita la interrupcion por registro vacio
	//	UCSZ0(2:1) Pertenece al registro UCSZ (011) que define los bits del dato, el valor del caracter (8 bits)
	//	RXB80 Inhabilita la recepcion de datos para el noveno bit en caso de configurar para 9 bits
	//	TXB80 Inhabilita la transmision de datos para el noveno bit en caso de configurar para 9 bits
	UCSR0B |= (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);
	//	RXCIE0 Habilita las interrupciones por recepcion de datos
	//	RXEN0 Habilita la recepcion de datos
	//	TXEN0 Habilita la transmision de datos
	
	//	-	-	-	-	-	Configuracion del Registro C	-	-	-	-	-
	UCSR0C &=~ (1<<UMSEL01)&(1<<UMSEL00)&(1<<UPM01)&(1<<UPM00)&(1<<USBS0)&(1<<UCPOL0);
	//	UMSEL0 (1:0) Selecciona el modo de operacion del USART (asincrono)
	//	UPM0 (1:0) Selecciona el modo de paridad desactivada
	//	USBS0 Selecciona los bits de parada (1 bit)
	//	UCPOL0 Desactiva la paridad del clock dado que se usara el modo asincrono
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
	//	UCSZ0(2:1) Pertenece al registro UCSZ (011) que define los bits del dato, el valor del caracter (8 bits)
	
	//	Calculo del BAUDRATE
	UBRR0 = BAUDRATE_UBRR0;

// 	#if DEBUG
// 	UART_write("Si anda jalando");
// 	#endif
}

unsigned char uart_read(){
	if(UCSR0A&(1<<RXC0)){		// Si el bit7 del registro UCSR0A se ha puesto a 1
		return UDR0;			//	Devuelve el dato almacenado en el registro UDR0
	}
	else
	return 0;
}

void uart_write(char *caracter){	//cadena de caracteres de tipo char
    while (*caracter) {
        while (!(UCSR0A & (1 << UDRE0)));	// Mientras el último valor de la cadena sea diferente a el caracter nulo
        UDR0 = *caracter;					// Manda los caracteres de manera individual
        caracter ++;						//incrementa la ubicación de los caracteres en cadena para enviar el siguiente caracter de cadena
    }
    while (!(UCSR0A & (1 << UDRE0)));		// Espera mientras el registro UDR0 este lleno
    UDR0 = '\n';							// Manda una nueva linea para estar seguro
}

void uart_writeln(char *caracter){	//cadena de caracteres de tipo char
	while (*caracter) {
		while (!(UCSR0A & (1 << UDRE0)));	// Mientras el último valor de la cadena sea diferente a el caracter nulo
		UDR0 = *caracter;					// Manda los caracteres de manera individual
		caracter ++;						//incrementa la ubicación de los caracteres en cadena para enviar el siguiente caracter de cadena
	}
	while (!(UCSR0A & (1 << UDRE0)));		// Espera mientras el registro UDR0 este lleno
	UDR0 = '\r';							// Hace un salto de carro
	UDR0 = '\n';							// Manda una nueva linea para estar seguro
}



#endif /* UART_H_ */