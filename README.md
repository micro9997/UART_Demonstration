
# UART_Demonstration
UART Demonstration using ATmega328P

&nbsp;

### 01. Introduction

*Stands for universal asynchronous receiver / transmitter and defines a protocol, or set of rules, for exchanging serial data between two devices. UART is very simple and only uses two wires between transmitter and receiver to transmit and receive in both directions.*

&nbsp;

### 02. Methodology

![image_1](https://github.com/micro9997/UART_Demonstration/blob/master/images/image_1.jpeg)

&nbsp;

***ATmega328P Register Description for UART***

* UDRn – USART I/O Data Register n

    *The USART Transmit Data Buffer Register and USART Receive Data Buffer Registers share the same I/O address referred to as USART Data Register or UDRn.*

* UCSRnA – USART Control and Status Register n A
    * *Bit 7 – RXCn: USART Receive Complete*
    * *Bit 6 – TXCn: USART Transmit Complete*
    * *Bit 5 – UDREn: USART Data Register Empty*
    * *Bit 4 – FEn: Frame Error*
    * *Bit 3 – DORn: Data OverRun*
    * *Bit 2 – UPEn: USART Parity Error*
    * *Bit 1 – U2Xn: Double the USART Transmission Speed*
    * *Bit 0 – MPCMn: Multi-processor Communication Mode*

* UCSRnB – USART Control and Status Register n B
    * *Bit 7 – RXCIEn: RX Complete Interrupt Enable n*
    * *Bit 6 – TXCIEn: TX Complete Interrupt Enable n*
    * *Bit 5 – UDRIEn: USART Data Register Empty Interrupt Enable n*
    * *Bit 4 – RXENn: Receiver Enable n*
    * *Bit 3 – TXENn: Transmitter Enable n*
    * *Bit 2 – UCSZn2: Character Size n*
    * *Bit 1 – RXB8n: Receive Data Bit 8 n*
    * *Bit 0 – TXB8n: Transmit Data Bit 8 n*

* UCSRnC – USART Control and Status Register n C
    * *Bits 7:6 – UMSELn1:0 USART Mode Select*
    * *Bits 5:4 – UPMn1:0: Parity Mode*
    * *Bit 3 – USBSn: Stop Bit Select*
    * *Bit 2:1 – UCSZn1:0: Character Size*
    * *Bit 0 – UCPOLn: Clock Polarity*

* UBRRnL and UBRRnH – USART Baud Rate Registers
    * *Bit 15:12 – Reserved*
    * *Bit 11:0 – UBRR[11:0]: USART Baud Rate Register*

&nbsp;

*Setup the baud rate*

```c
// Setup baud rate
#define F_CPU   16000000
#define BAUD    9600
#define MYUBRR  F_CPU/16/BAUD-1

// Write the baud rate to the UBRRnL and UBRRnH registers
UBRR0H = (MYUBRR >> 8);
UBRR0L = MYUBRR;
```

&nbsp;

*Enable the Tx and Rx*

```c
// Rx and Tx Enable
UCSR0B |= ((1 << TXEN0) | (1 << RXEN0));
```

&nbsp;

*Enable Rx interrupt*

```c
// RX Complete Interrupt Enable
UCSR0B |= (1 << RXCIE0);
```

&nbsp;

*Enable Global interrupt*

```c
// Global interrupt enable
SREG |= (1 << 7);
```

&nbsp;

*Read data*

```c
ISR(USART_RX_vect) {
    // Store the received data
    rxData = UDR0;
}
```

&nbsp;

*Write data*

```c
// Wait for Tx buff empty flag
while((UCSR0A & (1 << UDRE0)) == 0);

// Store the data to Tx buffer
UDR0 = data;
```

&nbsp;

### 03. Evaluation and Result

[![image_2](https://github.com/micro9997/UART_Demonstration/blob/master/images/image_2.png)](https://youtu.be/vCvw43UV1TY)

&nbsp;

### 04. Conclusion

Advantages
* Easy to interface.
* Less hardware: only 2 wires are required.
* There are no software complications like addressing the slaves.

Disadvantages
* Low speed other than SPI, I2C and USB.
* should match the baud rate of the sender and receiver.
* It's only intended for peer-to-peer communication and cannot connect to a third device.
* It doesn't contain an acknowledgement mechanism, so it's hard to check whether the data is received by the receiver successfully.

&nbsp;

### 04. Appendix

*node_1.c*
```C
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
```

*node_2.c*
```C
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
```

&nbsp;
