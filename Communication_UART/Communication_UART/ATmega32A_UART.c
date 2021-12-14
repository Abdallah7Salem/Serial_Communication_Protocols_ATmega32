/*
 * ATmega32A_UART.c
 *
 * Created: 11/22/2021 4:50:47 AM
 *  Author: Abdallah
 */ 

/* ====================== Includes ==================== */
#include "ATmega32A_UART.h"
#include "avr/io.h"

/* Configurations Style 
 *
 * BAUD = 9600
 * Stop = 1 Stop
 * Parity = Disabled
 * Data = 7 bit
 * Asynchronous Mode 
 */

/* ====================== Functions Defnitions ======================== */

/**======================================================================
 * @Fn				-UART_Init
 * @brief			-Initialize the UART of ATmega32A Micro controller
 * @parameter[in]	-None
 * @return value    -None
 * Note				-None
 */
void UART_Init (void)
{
	// According to The Equation in Data Sheet UBRR must be Set to 12 to have a 9600 BaudRate
	UBRRH = 0;
	UBRRL = 12;
	
	// Enable Double Speed Communication
	UCSRA = (1 << U2X);
	
	// Select Data Frame Width to 7-bit
	UCSRC = (1 << URSEL) | (1 << UCSZ1);
	
	// Enable UART Receive and Transmit Pins
	UCSRB = (1 << RXEN) | (1 << TXEN);
}

/**======================================================================
 * @Fn				-UART_SendByte
 * @brief			-Functions that sends byte through UART
 * @parameter[in]	-unsigned int Byte : The byte that it will send 
 * @return value    -None
 * Note				-None
 */
void UART_SendByte (unsigned char Byte)
{
	/* In Order To Send A Byte We Must Be Sure That Buffer Is Empty
	 * RXC Flag  : It is Set to 1 When UART Receive Complete
	 * TXC Flag  : It is Set to 1 When UDR Register is Empty & Shift Register Empty
	 * UDRE Flag : It is Set to 1 When only UDR Register is Empty
	 * We will Check Only The IDR Register
	 */
	
	// Polling Mechanism
	while ((UCSRA & (1<<UDRE)) == 0)						// UART Buffer Is Busy
	{
		// Do Nothing / Wait Till UDR Register Is Empty
	}
	
		// Send Byte To UDR Register
		UDR = Byte;
	
}

/**======================================================================
 * @Fn				-UART_ReceiveByte
 * @brief			-Functions that Receives byte through UART
 * @parameter[in]	-None
 * @return value    -unsigned char : the character that has been received by UART
 * Note				-None
 */
unsigned char UART_ReceiveByte (void)
{
	// RXC : UART Receive Complete
	// This Flag Bit Is Set When There Are Unread Data In The Receive
	
	// Polling Mechanism
	while ((UCSRA & (1<<RXC)) == 0)							// Receive Register Is Busy
	{
		// Do Nothing / Wait Till RXC Register Is Empty
	}
	
	return UDR;
}