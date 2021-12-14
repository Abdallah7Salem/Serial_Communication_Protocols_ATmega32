/*
 * MP3_Module.h
 *
 * Created: 12/14/2021 2:29:44 PM
 *  Author: Abdallah
 */ 


#ifndef MP3_MODULE_H_
#define MP3_MODULE_H_

/* ======================================== Includes ================================================== */
#include "ATmega32A_UART.h"

/* ======================================== Defintions ================================================ */
#define MP3_COMMAND_SIZE			10
#define MP3_START_BYTE				0x7E
#define MP3_VERSION					0xFF

#define MP3_NO_FEEDBACK				0x00
#define MP3_REQUEST_FEEDBACK		0x01

#define MP3_END_BYTE				0xEF

// List Of MP3 Commands 
#define MP3_CMD_PLAYBACK			0x03
#define MP3_CMD_SET_VOLUME			0x06
#define MP3_CMD_RESET				0x0C
#define MP3_CMD_PLAY				0x0D
#define MP3_CMD_PAUSE				0x0E


/* ======================================== Global Variables ========================================== */
// An array to store MP3 Command Values / Data
unsigned char Mp3_DefaultCommand[MP3_COMMAND_SIZE] = {MP3_START_BYTE, MP3_VERSION, 6, 0, MP3_NO_FEEDBACK, 0, 0, 0, 0, MP3_END_BYTE};

/* ======================================== Functions Prototypes ======================================= */

// Initialize the MP3 Module
void Mp3_Init(void);

// We Play Tracks Using This Function by Passing Track To It .
void Mp3_PlayTrack(unsigned short TrackNumber);

// We determine Volume Level Using This Function
void Mp3_SelectVolume(unsigned short VolumeLevel);



#endif /* MP3_MODULE_H_ */