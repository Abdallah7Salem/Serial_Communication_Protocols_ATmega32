/*
 * ATmega32A_UART.h
 *
 * Created: 11/29/2021 11:15:51 AM
 *  Author: Abdallah
 */ 


/*
 *		Preprocessor Configurable Driver
 *		Linking Configurable Driver
 *		PostBuild Configurable Driver "During RunTime"
 */

#ifndef ATMEGA32A_UART_H_
#define ATMEGA32A_UART_H_

/* =========================== USART Mode For Processor ============== */
	typedef enum
	{
		USART_SingleProcessor,
		USART_MultiProcessor,
	}USART_CommunicationTerminal;

/* =============== USART Communication Mode(Synchronous/Asynchronous) & Speed =================== */
	typedef enum
	{
		// Asynchronous Mode ---> Speed
		USART_Async_NormalSpeed,
		USART_Async_DoubleSpeed,
		
		// Synchronous Mode ---> Speed
		USART_Sync,	
	}USART_CommunicationMode;
	
/* ================== USART Modes ===================== */	
	typedef enum
	{
		USART_Disable,											// Disable USART
		USART_Tx_Only,											// Enable only Transmit Mode
		USART_Rx_Only,											// Enable only Receive Mode
		USART_Tx_Rx,											// Enable Both Transmit and Receive Modes
	}USART_EnableMode;

/* =================== USART Interrupt Source ======================== */
	typedef enum
	{
		// We Have 3 Interrupt Bits So We Have 8 Possibilities	
		USART_InterruptDisable,									// Disable Interrupts Of The USART						
		USART_RxComplete_Enable,								// Enable Interrupt when Receive Is Complete
		USART_TxComplete_Enable,								// Enable Interrupt when Transmit Is Complete
		USART_UDREmpty_Enable,										// Enable Interrupt when UDR Register is Empty
		USART_RxComplete_TxComplete_Enable,						// Enable Interrupt when Both Receive and Transmit are Complete
		USART_RxComplete_UDREmpty_Enable,						// Enable Interrupt when Both Receive is Complete and UDR Register is Empty
		USART_TxComplete_UDREmpty_Enable,						// Enable Interrupt when Both Transmit is Complete and UDR Register is Empty
		USART_RxComplete_TxComplete_UDREmpty,					
	}USART_InterruptSource;
	
	
/* ================== USART Data Frame ====================== */
	typedef enum
	{
		USART_5bitData,
		USART_6bitData,
		USART_7bitData,
		USART_8bitData,
		USART_9bitDAta,
	}USART_DataFrame;
	
/* ================ USART Control Frame ===================== */
	typedef enum
	{
		// We have 3 bits : Even, Odd, StopBit  ----> 8 Outcomes
		USART_ParityDisable_1Stop,								// USART Parity Disabled , USART Sends 1 Stop bit
		USART_ParityDisable_2Stop,								// USART Parity Disabled , USART Sends 2 Stop bits
		USART_ParityEven_1Stop,									// USART Parity Enabled Even , USART Sends 1 Stop bit
		USART_ParityEven_2Stop,									// USART Parity Enabled Even , USART Sends 2 Stop bits
		USART_ParityOdd_1Stop,									// USART Parity Enabled Odd , USART Sends 1 Stop bit
		USART_ParityOdd_2Stop,									// USART Parity Enabled Odd , USART Sends 2 Stop bits
	}USART_ControlFrame;

/* ============== UART Clock Polarity ====================== */
	typedef enum
	{
		USART_SampleOnFalling,
		USART_SampleOnRising,
	}USART_ClockPolarity;

/* ============= UART Error Type  ========================== */
typedef enum
{
	USART_NOError,
	USART_ParityError,
	USART_OverRunError,
	USART_FrameError,
}USART_ErrorType;
/* ================================================================================================================================ */
	typedef struct  
	{
		// Please Refer To Upper Enumerations For Details On What To Select
		USART_EnableMode				EnableMode;
		USART_CommunicationMode			CommunicationMode;
		USART_CommunicationTerminal		CommunicationTerminal;
		USART_InterruptSource			InterruptSource;
		USART_DataFrame					DataFrame;
		USART_ControlFrame				Control_Frame;
		// Assign Clock Polarity Option Only When Working in Synchronous Mode
		USART_ClockPolarity				ClockPolarity;
		unsigned long					BaudRate;		// 10-bit Variable
		// Read This Flag Before Using Data Returned by Read Function , Clear Flag Your Self
		USART_ErrorType					ReadMsg_Error;
		// CallBack Members
		// Make The Pointer Points to a Function you wish to be Called by RXC Interrupt
		void							(*RXC_InterruptCall)(void);
		// Make a pointer to a function you wish to be called by TXC Interrupt
		void							(*TXC_InterruptCall)(void);
		// Make a pointer to a function you wish to be called by UDRE Interrupt
		void							(*UDRE_InterruptCall)(void);
	}USART_ConfigStruct;


/* =================================== Function Prototypes ========================================= */
void USART_Init(void);
void USART_SendByte_Blocking(unsigned short Data);
void USART_SendByte_NonBlocking(unsigned short Data);
unsigned short USART_ReadByte_Blocking(void);
unsigned short USART_ReadByte_NonBlocking(void);

/* ================================== Global Variables ============================================= */
extern USART_ConfigStruct UART0;

#endif /* ATMEGA32A_UART_H_ */