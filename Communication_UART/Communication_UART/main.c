/*
 * Communication_UART.c
 *
 * Created: 11/22/2021 4:48:59 AM
 * Author : Abdallah
 */ 

#define F_CPU (1000000UL)
#include <avr/io.h>
#include "ATmega32A_UART.h"

int main(void)
{
	// Initializing UART Peripheral/Module
	UART_Init();
	
	// Defining Test Variable
	char testVariable = 'a';
    
    while (1) 
    {
		// Test : Circular Buffer To Make Sure That UART Send/Receive Functions Work Properly
		UART_SendByte(testVariable);
		// Receiving The Data Sent in The Same testVariable Then Send testVariable Again !!
		testVariable = UART_ReceiveByte();
    }
}

