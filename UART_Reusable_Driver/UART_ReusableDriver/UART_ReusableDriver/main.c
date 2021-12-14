/*
 * UART_ReusableDriver.c
 *
 * Created: 11/29/2021 11:14:17 AM
 * Author : Abdallah
 */ 

#include <avr/io.h>
#include "ATmega32A_UART.h"

int main(void)
{
	UART0.BaudRate = 9600;
	
    /* Replace with your application code */
    while (1) 
    {
    }
}

