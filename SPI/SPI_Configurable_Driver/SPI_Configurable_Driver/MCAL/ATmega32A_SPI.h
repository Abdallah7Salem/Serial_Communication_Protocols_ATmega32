/*
 * ATmega32A_SPI.h
 *
 * Created: 1/26/2022 4:25:21 AM
 *  Author: Abdallah
 */ 


#ifndef ATMEGA32A_SPI_H_
#define ATMEGA32A_SPI_H_

	typedef enum
	{
		SPI_Disable,
		SPI_Enable,
	}SPI_Enable_or_Disable;
	
	typedef enum
	{
		SPI_InterruptDisable,
		SPI_InterruptEnable,
	}SPI_InterruptState;
	
	typedef enum
	{
		SPI_MSB,
		SPI_LSB,
	}SPI_Data_Order;

	typedef enum
	{
		SPI_Slave,
		SPI_Master,
	}SPI_Select_Master_or_Slave;

	typedef enum
	{
		SPI_CLK_Mode0,
		SPI_CLK_Mode1,
		SPI_CLK_Mode2,
		SPI_CLK_Mode3,
	}SPI_Clock_Modes;
	
	typedef enum
	{
		SPI_CPU_Frequency_Divided_by_2,
		SPI_CPU_Frequency_Divided_by_4,
		SPI_CPU_Frequency_Divided_by_8,
		SPI_CPU_Frequency_Divided_by_16,
		SPI_CPU_Frequency_Divided_by_32,
		SPI_CPU_Frequency_Divided_by_64,
		SPI_CPU_Frequency_Divided_by_128,
	}SPI_Frequency;
	
	typedef enum
	{
		SPI_No_Error,
		SPI_Write_Collision,
	}SPI_Errors;
	
	typedef struct  
	{
		SPI_Enable_or_Disable	Enable_Control;
		SPI_Select_Master_or_Slave	Mode;
		SPI_Clock_Modes	Clock_Mode;
		SPI_Data_Order	Data_Order;
		SPI_InterruptState	InterruptState;
		
		void (*SPI_Transfer_Interrupt_Call) (void);
		
		/* For Slave Mode --> Frequency shouldn't be selected */
		SPI_Frequency	Frequency;
		
		/* This bit doesn't clear itself , Check it before using Data and erase 
		 * the error yourself by assigning it to SPI_No_Error 
		 */
		SPI_Errors	Error;
	}SPI_Config_Struct;
	
	extern SPI_Config_Struct SPI0;
	
	void SPI_Init(void);
	unsigned char SPI_Read_or_Write_Blocking(unsigned char Data);
	unsigned char SPI_Read_or_Write_NonBlocking(unsigned char Data);
	
#endif /* ATMEGA32A_SPI_H_ */