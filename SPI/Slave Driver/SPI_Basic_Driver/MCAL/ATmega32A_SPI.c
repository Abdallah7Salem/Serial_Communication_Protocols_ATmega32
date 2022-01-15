/*
 * ATmega32A_SPI.c
 *
 * Created: 1/8/2022 1:34:30 AM
 *  Author: Abdallah
 */ 

#include <avr/io.h>
#include "Atmega32A_SPI.h"
#include "uc_Configuration.h"

#define MOSI 5
#define SS 4
#define SCK 7
#define MISO 6

/* --- Assumptions ---
 *	SPI as Slave
 *  Frequency = 250KHz
 *  SPI Mode 0
 *  MSB First
 */
void SPI_Initiate(void)
{
	SPCR |= (1 << SPE) | (1 << MSTR);
	DDRB |= (1 << MISO);
}

void SPI_SendByte(unsigned char Data)
{
	SPDR = Data;
	while((SPSR & (1 << SPIF)) == 0) {}
}

unsigned char SPI_ReadByte(void)
{
	unsigned char Dummy = 0x00;
	SPDR = Dummy;
	while((SPSR & (1 << SPIF)) == 0) {}
	return SPDR;
}

unsigned char SPI_SendAndReceive(unsigned char Data)
{
	SPDR = Data;
	while((SPSR & (1 << SPIF)) == 0) {}
	return SPDR;
}