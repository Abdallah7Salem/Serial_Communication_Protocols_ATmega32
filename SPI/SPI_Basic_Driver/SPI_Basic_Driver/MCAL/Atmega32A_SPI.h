/*
 * Atmega32A_SPI.h
 *
 * Created: 1/8/2022 1:33:50 AM
 *  Author: Abdallah
 */ 


#ifndef ATMEGA32A_SPI_H_
#define ATMEGA32A_SPI_H_


void SPI_Initiate(void);
void SPI_SendByte(unsigned char Data);
unsigned char SPI_ReadByte(void);

unsigned char SPI_SendAndReceive(unsigned char Data);

#endif /* ATMEGA32A_SPI_H_ */