/*
 * ATmega32A_UART.c
 *
 * Created: 11/29/2021 11:15:30 AM
 *  Author: Abdallah
 */ 

/* ============================================= Includes ==================================== */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "ATmega32A_UART.h"
#include "uc_Configuration.h"

/* ============================================= Defintions =================================== */
#define NULL_PTR						((void *)0x00)

/* ============================================ Global Variable ============================== */
USART_ConfigStruct	UART0 = {0,												// UART0 Because There Can Be More Than 1 UART
							 .RXC_InterruptCall = NULL_PTR,					// If the User doesn't make the pointer point to a function executed when there is CallBack
																			// The Pointer Will be NULL Pointer 
							 .TXC_InterruptCall = NULL_PTR,
							 .UDRE_InterruptCall = NULL_PTR };
																			
/* ============================================ Interrupt Functions ========================== */

// This is the Function which is Called Whenever There is an Interrupt
// We pass the interrupt vector number as a parameter to the ISR() Function

// _VECTOR(Vector No.) ; And You get the Vector No. From The Datasheet || USART_RXC_VECT
// 14 in Datasheet means 13 
// Inside the Function We will write the Code of the Interrupt of RXC
ISR(USART_RXC_vect)		// _VECTOR(13)
{
	// UART0 has a Pointer to Function Member 
	// Call The Pointed Function from that Pointer
	// Notice : The User Determines Which Function The Pointer Points At.
	
	if (UART0.RXC_InterruptCall != NULL_PTR)
	{
		UART0.RXC_InterruptCall();
	}
	else								// In case of the User doesn't Determine The Function Pointed At.
	{
		// Handle Error Using Error Handling Function
	}
}

// TXC Interrupt Service Routine
ISR(USART_TXC_vect)
{
	if (UART0.TXC_InterruptCall != NULL_PTR)
	{
		UART0.TXC_InterruptCall();
	}
	else
	{
		// Handle Error Using Error Handling Function
	}
}

// UDRE Interrupt Service Routine
ISR(USART_UDRE_vect)
{
	if (UART0.UDRE_InterruptCall != NULL_PTR)
	{
		UART0.UDRE_InterruptCall();
	}
	else
	{
		// Handle Error Using Error Handling Function
	}
}
/* ============================================ Functions Definitions ======================== */

/* =================================================================
 * @Fn					-USART_Init
 * @Brief				-Initialize The USART and Its Configurations
 * @Param[in]			-None
 * @retval				-None
 * @Note				-None
 */
void USART_Init (void)
{
	// These Variable Are Used To Put Values In Registers 
	// They Are Used Only In USART_Init Function
	unsigned char UCSRA_Temp = 0;
	unsigned char UCSRB_Temp = 0;
	unsigned char UCSRC_Temp = 0;
	
	// This Variable Is Used To Put The Value Of The BaudRate 
	float UBRR_Temp = 0;
	
	
	/* ======= Switch On Enable Mode For The USART ======= */
	switch (UART0.EnableMode)
	{
		case USART_Disable:
			// Do Nothing / We Are Resetting UCSRB_Temp = 0
			break;
		case USART_Rx_Only:
			UCSRB_Temp |= (1 << RXEN);									// Enable Receive Only On The USART
			break;
		case USART_Tx_Only:
			UCSRB_Temp |= (1 << TXEN);									// Enable Transmission Only On The USART
			break;
		case USART_Tx_Rx:
			UCSRB_Temp |= (1 << RXEN) | (1 << TXEN);						// Enable Both Transmission And Receive On The USART
			break;
		default:
			break;
	}
	
	/* ====== Switch On The Communication Mode For The USART ======== */
	switch (UART0.CommunicationMode)
	{
		case USART_Async_NormalSpeed:
			UBRR_Temp = (((float)F_CPU) / (16.0 * UART0.BaudRate)) - 0.5;		// -1 + 0.5 = -0.5 : as the register accepts only int values 
																				// so it approximates for example 11.9 to 11 this is wrong
																				// we added +0.5 to make it approximate correctly .
			break;
		case USART_Async_DoubleSpeed:
			UBRR_Temp = (((float)F_CPU) / (8.0 * UART0.BaudRate)) - 0.5;		// In double speed : 8 instead of 16
			UCSRA_Temp |= (1 << U2X);
			break;
		case USART_Sync:
			UCSRC_Temp |= (1 << UMSEL);
			UBRR_Temp = (((float)F_CPU) / (2.0 * UART0.BaudRate)) - 0.5;
			
			/* ====== Switch On Clock Polarity In Synchronous Mode ====== */
			switch (UART0.ClockPolarity)
			{
				case USART_SampleOnFalling:
					break;
				case USART_SampleOnRising:
					UCSRC_Temp |= (1<<UCPOL);
					break;
			}
			break;
		default:
			break;
	}
	
	/* ===== Switch On The Communication Terminal For The USART ======= */
	switch(UART0.CommunicationTerminal)
	{
		case USART_SingleProcessor:
			break;
		case USART_MultiProcessor:
			UCSRA_Temp |= (1 << MPCM);
			break;
		default:
			break;
	}
	
	/* ===== Switch On The Interrupt Sources For The USART ======== */
	switch (UART0.InterruptSource)
	{
		case USART_InterruptDisable:											// Disable USART Interrupt
			break;
		case USART_RxComplete_Enable:											// Enable Receive Complete Interrupt 
			UCSRB_Temp |= (1 << RXCIE);
			break;
		case USART_TxComplete_Enable:											// Enable Transmit Complete Interrupt
			UCSRB_Temp |= (1 << TXCIE);
			break;
		case USART_UDREmpty_Enable:												// Enable UDR Empty Interrupt
			UCSRB_Temp |= (1 << UDRIE);
			break;
		case USART_RxComplete_TxComplete_Enable:								// Enable Both Transmit/Receive Interrupts
			UCSRB_Temp |= (1 << RXCIE) | (1 << TXCIE);
			break;
		case USART_RxComplete_UDREmpty_Enable:									// Enable Both Receive Complete & UDR Empty Interrupts
			UCSRB_Temp |= (1 << UDRIE) | (1 << RXCIE);
			break;
		case USART_TxComplete_UDREmpty_Enable:									// Enable Transmit Complete & UDR Empty Interrupts
			UCSRB_Temp |= (1 << TXCIE) | (1 << UDRIE);
			break;
		case USART_RxComplete_TxComplete_UDREmpty:								// Enable Receive Complete & UDR Empty Interrupts
			UCSRB_Temp |= (1 << RXCIE) | (1 << TXCIE) | (1 << UDRIE);
			break;
		default:
			break;
	}
	
	/* ===== Switch On The Data Frame For The USART ======== */
	switch(UART0.DataFrame)
	{
		case USART_5bitData:													// Enable 5 bit Data Frame For USART
			break;
		case USART_6bitData:													// Enable 6 bit Data Frame For USART
			UCSRC_Temp |= (1<<UCSZ0);
			break;
		case USART_7bitData:													// Enable 7 bit Data Frame For USART
			UCSRC_Temp |= (1<<UCSZ1);
			break;
		case USART_8bitData:													// Enable 8 bit Data Frame For USART
			UCSRC_Temp |= (1<<UCSZ0) | (1<<UCSZ1);
			break;
		case USART_9bitDAta:													// Enable 9 bit Data Frame For USART
			UCSRC_Temp |= (1<<UCSZ0) | (1<<UCSZ1);
			UCSRB_Temp |= (1<<UCSZ2);
			break;
		default:
			break;
		
	}
	
	/* ===== Switch On The Control Frame For The USART ======== */
	switch(UART0.Control_Frame)
	{
		case USART_ParityDisable_1Stop:
			break;
		case USART_ParityDisable_2Stop:
			UCSRC_Temp |= (1<<USBS);
			break;
		case USART_ParityEven_1Stop:
			UCSRC_Temp |= (1<<UPM1);
			break;
		case USART_ParityEven_2Stop:
			UCSRC_Temp |= (1<<USBS) | (1<<UPM1);
			break;
		case USART_ParityOdd_1Stop:
			UCSRC_Temp |= (1<<UPM0) | (1<<UPM1);
			break;
		case USART_ParityOdd_2Stop:
			UCSRC_Temp |= (1<<USBS) | (1<<UPM0) | (1<<UPM1) ;
			break;
			
	}
	
	// Putting Values Of Variables Defined Above In Their Registers
	UBRRH = ((unsigned short)UBRR_Temp)>>8;
	UBRRL = ((unsigned char)UBRR_Temp);
	UCSRA = UCSRA_Temp;
	UCSRC = UCSRC_Temp;
	UCSRB = UCSRB_Temp;												// USART works on the Transmit and Receive Registers in UCSRB Register
	
}

/* =================================================================
 * @Fn					-USART_SendByte_Blocking
 * @Brief				-Send Data using the blocking mode which is polling ( it waits till the UDRE becomes empty to send the Data )
 * @Param[in]			-unsigned short Data
 * @retval				-None
 * @Note				-None
 */
void USART_SendByte_Blocking(unsigned short Data)					   	   // unsigned short (16-bit) : as data can be 9-bit frame
{
	// Wait for the register to be empty
	// If it is empty --> Put Data in UDR
	while ((UCSRA & (1<<UDRE)) == 0 )
	{
		// Do nothing till the register become empty
	}
	
	if (UART0.DataFrame == USART_9bitDAta)
	{
		// Put the 9th bit first then put the 8-bits together
		// (UCSRB & 0xFE) : Reset Bit0 in the register
		UCSRB = (UCSRB & 0xFE) | ((Data & (1<<8)) >> 8);										// 1 0000 0000 >> 8
		/*
		 * Illustration :
		 *
		 *		Data Sent : 1 1001 0001 (9 bit data)
		 *		
		 *		1 1001 0001 & (1<<8)	=	1 0000 0000
		 * 
		 *		1 0000 0000 >> 8		=	0 0000 0001 = 0000 00001
		 */
	}
	
	// Send the remaining / main 8 bit data
	UDR = (unsigned char) Data;									// Data parameter is 16-bit, so we cast it into 8-bit as we need only 8-bits 
}

/* =================================================================
 * @Fn					-USART_SendByte_NonBlocking
 * @Brief				-Send Data using interrupt mode , this function will be called in the
 *						 UDRE_ISR() or TXC_ISR() and in both cases UDRE will be empty
 * @Param[in]			-unsigned short Data
 * @retval				-None
 * @Note				-None
 */
void USART_SendByte_NonBlocking(unsigned short Data)
{
	if((UCSRA & (1<<UDRE)) != 0)
	{
		if (UART0.DataFrame == USART_9bitDAta)
		{
			UCSRB = (UCSRB & 0xFE)	| ((Data & (1<<8)) >> 8);
		}
		UDR = (unsigned char) Data;
	}
}

/* =================================================================
 * @Fn					-USART_ReadByte_Blocking
 * @Brief				-Read byte using polling ( it waits till the data is sent to it || RXC Flag is Set )
 * @Param[in]			-None
 * @retval				-unsigned short Data ; because DataFrame can be 9-bit 
 *						 we cast it inside the function for 8-bit mode
 * @Note				-None
 */
unsigned short USART_ReadByte_Blocking(void)
{
	// Variable to receive data ( receive the 9th bit ) in case of 9-bit DataFrame
	unsigned short Rx_Data = 0;
	
	// Check if Receive is Complete
	while ( (UCSRA & (1<<RXC)) == 0 )
	{
		// Do nothing till the current receive is complete --> RXC Flag will be 1 
	}
	
	// Check if there  is an error in the data 
	// using : FE , DOR , PE
	// If one of the above Flags is set , It means that there is an error found 
	if (UCSRA & ( (1<<FE) | (1<<DOR) | (1<<UPE) ))
	{
		// Error : You should handle it as you Wish
		
		if (UCSRA & (1<<FE))							// In case of Frame Error
		{
			UART0.ReadMsg_Error = USART_FrameError;
		}
		else if (UCSRA & (1<<DOR))						// In case of OverRunError
		{
			UART0.ReadMsg_Error = USART_OverRunError;
		}
		else											// In case of Parity Error
		{
			UART0.ReadMsg_Error = USART_ParityError;
		}
		
	}
	
	// In case of there are no errors
	if (UART0.DataFrame == USART_9bitDAta)
	{
		Rx_Data = (UCSRB & (1<<RXB8)) << 7;
	}
	
	Rx_Data |= UDR;
	
	return Rx_Data;
}

/* =================================================================
 * @Fn					-USART_ReadByte_NonBlocking
 * @Brief				-Read byte using interrupt ( It will be called inside ISR )
 * @Param[in]			-None
 * @retval				-unsigned short Data ; because DataFrame can be 9-bit 
 *						 we cast it inside the function for 8-bit mode
 * @Note				-None
 */
unsigned short USART_ReadByte_NonBlocking(void)
{
	unsigned short Rx_Data = 0;
	
	if ( (UCSRA & (1<<RXC)) != 0)
	{
		// Check if there  is an error in the data
		// using : FE , DOR , PE
		// If one of the above Flags is set , It means that there is an error found
		if (UCSRA & ( (1<<FE) | (1<<DOR) | (1<<UPE) ))
		{
			// Error : You should handle it as you Wish
			
			if (UCSRA & (1<<FE))							// In case of Frame Error
			{
				UART0.ReadMsg_Error = USART_FrameError;
			}
			else if (UCSRA & (1<<DOR))						// In case of OverRunError
			{
				UART0.ReadMsg_Error = USART_OverRunError;
			}
			else											// In case of Parity Error
			{
				UART0.ReadMsg_Error = USART_ParityError;
			}
			
		}
		
		// In case of there are no errors
		if (UART0.DataFrame == USART_9bitDAta)
		{
			Rx_Data = (UCSRB & (1<<RXB8)) << 7;
		}
		
		Rx_Data |= UDR;
	}
	return Rx_Data;
}