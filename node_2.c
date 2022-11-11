
#include <avr/io.h>
#include <avr/interrupt.h>

#include "writePWM.h"

#ifndef __AVR_ATmega328P__
    #include <avr/iom328p.h>
#endif

#define F_CPU   16000000
#define BAUD    9600
#define MYUBRR  F_CPU/16/BAUD-1

#define TX_BYTES  1
#define RX_BYTES  2

uint8_t txData = 0;
uint16_t rxData = 0;

uint8_t dataReceivedFlag = 0;

void tx(uint8_t data) {
    // Wait for Tx buff empty flag
    while((UCSR0A & (1 << UDRE0)) == 0);

    // Store the data to Tx buffer
    UDR0 = data;
}

ISR(USART_RX_vect) {
    static uint16_t data = 0;
    static uint8_t i_ISR = 0;
    data = data << (i_ISR * 8);
    data = data | UDR0;
    i_ISR++;
    if(i_ISR == RX_BYTES) {
        rxData = data;
        data = 0;
        dataReceivedFlag = 1;
        i_ISR = 0;
    }
}

void uartInit() {
    // Baud rate
    UBRR0H = (MYUBRR >> 8);
    UBRR0L = MYUBRR;

    // Receiver Enable
    UCSR0B |= ((1 << RXEN0) | (1 << TXEN0));

    // RX Complete Interrupt Enable
    UCSR0B |= (1 << RXCIE0);

    // Global interrupt enable
    SREG |= (1 << 7);
}

int main(void) {
    uartInit();
    initPWM_PB1();
    DDRD &= ~(1 << PIND5);

    uint8_t buttonFlag = 1;

    while(1) {
        if(dataReceivedFlag == 1) {
            uint8_t temp = rxData / 4;
            writePWM_PB1(temp);

            dataReceivedFlag = 0;
        }
        if((PIND & (1 << PIND5)) != 0 && buttonFlag == 1) {
            txData ^= (1 << 0);
            tx(txData);
            buttonFlag = 0;
        } else if((PIND & (1 << PIND5)) == 0 && buttonFlag == 0) {
            buttonFlag = 1;
        }
    }

    return 0;
}
