# UART_Demonstration
UART Demonstration using ATmega328P

&nbsp;

### 01. Introduction

*Stands for universal asynchronous receiver / transmitter and defines a protocol, or set of rules, for exchanging serial data between two devices. UART is very simple and only uses two wires between transmitter and receiver to transmit and receive in both directions.*

&nbsp;

### 02. Methodology

![image_1](https://github.com/micro9997/UART_Demonstration/blob/master/images/image_1.jpeg)

&nbsp;

***Register Description***

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



&nbsp;

### 03. Evaluation and Result

[![image_2](https://github.com/micro9997/UART_Demonstration/blob/master/images/image_2.png)](https://youtu.be/vCvw43UV1TY)

&nbsp;

### 04. Conclusion

Advantages
* Spped
* 

Disadvantages
* Spped
* 


&nbsp;
