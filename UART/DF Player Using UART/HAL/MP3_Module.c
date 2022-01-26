/*
 * MP3_Module.c
 *
 * Created: 12/14/2021 2:29:09 PM
 *  Author: Abdallah
 */ 

/* ============================================ Includes =========================================== */
#include "uc_Configuration.h"
#include "MP3_Module.h"
#include <util/delay.h>

/* ============================================ Functions Definitions =============================== */

/* =================================================================
 * @Fn					-Mp3_SendCommandFrame
 * @Brief				-Send the Default CMD ( Mp3_DefaultCommand) Through The UART.
 * @Param[in]			-None
 * @retval				-None
 * @Note				-None
 */
static void Mp3_SendCommandFrame(void)
{
	unsigned char i = 0;
	unsigned short CheckSum				= 0;
	unsigned short Addition_Of_Frame    = 0;
	
	for (i = 0; i < 10; i++)
	{
		// Add The Sent Value To The Addition_Of_Frame
		//Addition_Of_Frame += Mp3_DefaultCommand[i];
		
		if (i > 0 && i < 7)
		{
			Addition_Of_Frame += Mp3_DefaultCommand[i];
		}
		else if (i == 7)		// Here We Calculate The CheckSum
		{
			// We Got The Equation From The Datasheet
			CheckSum = 0xFFFF - Addition_Of_Frame + 1;
			
			// MSB, LSB of CheckSum is Member 7,8
			// Access it through array
			
			// Suppose CheckSum is xxxx xxxx xxxx xxxx 
			// First xxxx xxxx : 7
			// Last xxxx xxxx : 8
			Mp3_DefaultCommand[7] = CheckSum >> 8;
			Mp3_DefaultCommand[8] = (unsigned char) CheckSum;  // We cast it to 1 byte because it is defined as 
															   // 2 byte and here we only need 1 byte .
		}
		
		// Send The Byte : We Put it After The If Condition
		// Because If We Modified The CheckSum Data, It will
		// Send The Updated Value Here.
		USART_SendByte_Blocking(Mp3_DefaultCommand[i]);
	}
}

/* =================================================================
 * @Fn					-Mp3_Init
 * @Brief				-Initialize the MP3 Module 
 * @Param[in]			-None
 * @retval				-None
 * @Note				-None
 */
void Mp3_Init(void)
{
	// First : We Configure USART During RunTime
	UART0.BaudRate					= 9600;
	UART0.EnableMode				= USART_Tx_Only;
	UART0.CommunicationMode			= USART_Async_DoubleSpeed;
	UART0.CommunicationTerminal		= USART_SingleProcessor;
	UART0.Control_Frame				= USART_ParityDisable_1Stop;
	UART0.DataFrame					= USART_9bitData;
	UART0.InterruptSource			= USART_InterruptDisable;
	
	// Second : Initialize The USART
	USART_Init();
	
	// Third : Set Mp3_Player to Ready to Receive Data
	// Reset The Mp3 CMD (Command)
	// Accessing Member 3 of The Array
	Mp3_DefaultCommand[3] = MP3_CMD_RESET;
	
	// Fourth : Send The Data
	Mp3_SendCommandFrame();
	
	// Return To Default Value 
	Mp3_DefaultCommand[3] = 0;
	
	_delay_ms(1000);
}

/* =================================================================
 * @Fn					-Mp3_PlayTrack
 * @Brief				-Play Track For The MP3 
 * @Param[in]			-unsigned short TrackNumber
 * @retval				-None
 * @Note				-None
 */
void Mp3_PlayTrack(unsigned short TrackNumber)
{
	Mp3_DefaultCommand[3] = MP3_CMD_PLAYBACK;
	Mp3_DefaultCommand[5] = TrackNumber >> 8;						// MSB
	Mp3_DefaultCommand[6] = (unsigned char)TrackNumber;				// LSB
	
	Mp3_SendCommandFrame();
	
	// Reset All The Parameters Again After Sending The Command
	Mp3_DefaultCommand[3] = 0;
	Mp3_DefaultCommand[5] = 0;
	Mp3_DefaultCommand[6] = 0;
	
	// We Store The Track Number in The LSB and MSB
}

/* =================================================================
 * @Fn					-Mp3_SelectVolume
 * @Brief				-Determines The Volume of The MP3
 * @Param[in]			-unsigned short VolumeLevel
 * @retval				-None
 * @Note				-None
 */
void Mp3_SelectVolume(unsigned short VolumeLevel)
{
	Mp3_DefaultCommand[3] = MP3_CMD_SET_VOLUME;
	Mp3_DefaultCommand[5] = VolumeLevel >> 8;
	Mp3_DefaultCommand[6] = (unsigned char) VolumeLevel;
	
	Mp3_SendCommandFrame();
	
	//Reset All The Parameters Again After Sending The Command
	Mp3_DefaultCommand[3] = 0;
	Mp3_DefaultCommand[5] = 0;
	Mp3_DefaultCommand[6] = 0;
}