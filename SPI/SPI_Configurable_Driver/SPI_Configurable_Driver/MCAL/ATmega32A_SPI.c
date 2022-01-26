/*
 * ATmega32_SPI.c
 *
 * Created: 1/26/2022 4:24:41 AM
 *  Author: Abdallah
 */ 
#include "Microcontroller_Configuration.h"
#include "ATmega32A_SPI.h"
#include <avr/io.h>
#include <avr/interrupt.h>

ISR(SPI_STC_vect)
{
	if (SPI0.SPI_Transfer_Interrupt_Call != NULL_PTR)
	{
		SPI0.SPI_Transfer_Interrupt_Call();
	}
}

	SPI_Config_Struct	SPI0 = {.SPI_Transfer_Interrupt_Call = NULL_PTR};
	
	void SPI_Init()
	{
		SPCR = 0;
		SPSR = 0;
		switch(SPI0.Enable_Control)
		{
			case SPI_Disable:
			break;
			
			case SPI_Enable:
				switch(SPI0.Mode)
				{
					SPCR |= (1<<SPE);
					case SPI_Master:
						SPCR |= (1<<MSTR);
						DDRB |= (1<<5) | (1<<7) | (1<<4);
					break;
					
					case SPI_Slave:
						DDRB |= (1<<6);
					break;
				}
				switch(SPI0.Data_Order)
				{
					case SPI_MSB:
					break;
					
					case SPI_LSB:
						SPCR |= (1<<DORD);
					break;
				}
				switch(SPI0.InterruptState)
				{
					case SPI_InterruptEnable:
						SPCR |= (1<<SPIE);
					break;
					
					case SPI_InterruptDisable:
					break;
				}
				switch(SPI0.Clock_Mode)
				{
					case SPI_CLK_Mode0:
					break;
					
					case SPI_CLK_Mode1:
						SPCR |= (1<<CPHA);
					break;
					
					case SPI_CLK_Mode2:
						SPCR |= (1<<CPOL);
					break;
					
					case SPI_CLK_Mode3:
						SPCR |= (1<<CPHA) | (1<<CPOL);
					break;
				}
				switch(SPI0.Frequency)
				{
					case SPI_CPU_Frequency_Divided_by_2:
						SPSR |= (1<<SPI2X);
					break;
					
					case SPI_CPU_Frequency_Divided_by_4:
					break;
					
					case SPI_CPU_Frequency_Divided_by_8:
						SPSR |= (1<<SPI2X);
						SPCR |= (1<<SPR0);
					break;
					
					case SPI_CPU_Frequency_Divided_by_16:
						SPCR |= (1<<SPR0);
					break;
					
					case SPI_CPU_Frequency_Divided_by_32:
						SPSR |= (1<<SPI2X);
						SPCR |= (1<<SPR1);
					break;
					
					case SPI_CPU_Frequency_Divided_by_64:
						SPCR |= (1<<SPR1);
					break;
					
					case SPI_CPU_Frequency_Divided_by_128:
						SPCR |= (1<<SPR0) | (1<<SPR1);
					break;
				}
			break;
		}
	}
	
	unsigned char SPI_Read_or_Write_Blocking(unsigned char Data)
	{
		/* Transmit Data using SPI Bus */
		SPDR = Data;
		
		/* Check if a Write Collision Error Occurred */
		if ( (SPSR & (1<<WCOL)) != 0)
		{
			SPI0.Error = SPI_Write_Collision;
		}
		
		/* Block till The End of Transmission */
		while ( (SPSR & (1<<SPIF)) == 0 ) {}
			
		/* return Rx Data from SPR */	
		return SPDR;
	}
	
	unsigned char SPI_Read_or_Write_NonBlocking(unsigned char Data)
	{
		/* Transmit Data using SPI Bus */
		SPDR = Data;
		
		/* Check if a Write Collision Error Occurred */
		if ( (SPSR & (1<<WCOL)) != 0)
		{
			SPI0.Error = SPI_Write_Collision;
		}
		
		/* return Rx Data from SPR */
		return SPDR;
	}