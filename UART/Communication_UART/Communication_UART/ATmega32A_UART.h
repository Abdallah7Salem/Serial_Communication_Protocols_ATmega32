/*
 * ATmega32A_UART.h
 *
 * Created: 11/22/2021 4:51:16 AM
 *  Author: Abdallah
 */ 


#ifndef ATMEGA32A_UART_H_
#define ATMEGA32A_UART_H_

/* ================= Definitions =================== */
#define UART_BAUDRATE		(9600)

/* ================= Functions Prototypes ==================== */

/**======================================================================
 * @Fn				-UART_Init
 * @brief			-Initialize the UART of ATmega32A Microcontroller
 * @parameter[in]	-None
 * @return value    -None
 * Note				-None
 */
void UART_Init (void);

/**======================================================================
 * @Fn				-UART_SendByte
 * @brief			-Functions that sends byte through UART
 * @parameter[in]	-unsigned int Byte : The byte that it will send 
 * @return value    -None
 * Note				-None
 */
void UART_SendByte (unsigned char Byte);

/**======================================================================
 * @Fn				-UART_ReceiveByte
 * @brief			-Functions that Receives byte through UART
 * @parameter[in]	-None
 * @return value    -unsigned char : the character that has been received by UART
 * Note				-None
 */
unsigned char UART_ReceiveByte (void);

#endif /* ATMEGA32A_UART_H_ */