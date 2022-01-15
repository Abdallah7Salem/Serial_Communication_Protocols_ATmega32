/*
 * SPI_Basic_Driver.c
 *
 * Created: 1/8/2022 1:30:05 AM
 * Author : Abdallah
 */ 

#include "uc_Configuration.h"
#include "Atmega32A_SPI.h"
#include "ATmega32A_UART.h"
#include "util/delay.h"


int main(void)
{
	UART0.EnableMode = USART_Tx_Rx;
	UART0.BaudRate = 9600;
	UART0.CommunicationMode = USART_Async_DoubleSpeed;
	UART0.Control_Frame = USART_ParityDisable_1Stop;
	UART0.DataFrame = USART_8bitData;
	UART0.InterruptSource = USART_InterruptDisable;
	UART0.CommunicationTerminal = USART_SingleProcessor;

	SPI_Initiate();	
	USART_Init();
    while (1) 
    {
		unsigned char Data = 0;
		Data = SPI_ReadByte();
		USART_SendByte_Blocking(Data);
    }
}

