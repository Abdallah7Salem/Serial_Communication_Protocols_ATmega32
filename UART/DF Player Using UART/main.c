/*
 * UART_ReusableDriver.c
 *
 * Created: 11/29/2021 11:14:17 AM
 * Author : Abdallah
 */ 

/* ========================================= Includes =============================================== */
#include "uc_Configuration.h"
#include <avr/io.h>
#include <util/delay.h>
#include "MP3_Module.h"

/* ========================================= Main Program ============================================ */
int main(void)
{
	// Initialize The MP3 Module 
	Mp3_Init();
	
	// Set The Volume To The Highest Value
	Mp3_SelectVolume(30);
	
	// Play The Track Number 2
	Mp3_PlayTrack(2);
	
	// Delay of 5 Seconds
	_delay_ms(5000);
	
	// Play The Track Number 1
	Mp3_PlayTrack(1);
	
    /* Replace with your application code */
    while (1) 
    {
    }
}

