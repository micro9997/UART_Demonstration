
#include <avr/io.h>
#include <avr/interrupt.h>

#include "readADC.h"

#ifndef __AVR_ATmega328P__
    #include <avr/iom328p.h>
#endif

#define F_CPU   16000000
#define BAUD    9600
#define MYUBRR  F_CPU/16/BAUD-1

#define TX_BYTES    2
#define RX_BYTES    1

uint16_t txData = 0;
uint8_t rxData = 0;

ISR(USART_RX_vect) {
    // Store the received data
    rxData = UDR0;
    if(rxData & 0b00000001) {
        PORTB |= (1 << PORTB1);
    } else {
        PORTB &= ~(1 << PORTB1);
    }
}

void tx(uint8_t data) {
    // Wait for Tx buff empty flag
    while((UCSR0A & (1 << UDRE0)) == 0);

    // Store the data to Tx buffer
    UDR0 = data;
}

void uartInit() {
    // Baud rate
    UBRR0H = (MYUBRR >> 8);
    UBRR0L = MYUBRR;

    // Rx and Tx Enable
    UCSR0B |= ((1 << TXEN0) | (1 << RXEN0));

    // RX Complete Interrupt Enable
    UCSR0B |= (1 << RXCIE0);

    // Global interrupt enable
    SREG |= (1 << 7);
}

int main(void) {
    uartInit();
    initADC0();
    DDRB |= (1 << PORTB1);

    uint16_t temp;

    while(1) {
        if((temp = readADC0()) != txData) {
            txData = temp;
            tx((txData >> 8));
            tx((txData >> 0));
        }
        
    }

    return 0;
}
