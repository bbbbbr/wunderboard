#ifndef WUNDERPROJECT_H
#define WUNDERPROJECT_H

#define TRUE  1
#define FALSE 0

//Define F_CPU (set in makefile to avoid redef warning)
#ifndef F_CPU_
#define F_CPU 1000000UL
#endif

//Lenght of Marguee text
#define STRING_SIZE 31

//Physical SPI pins
#define MISO	3
#define MOSI	2
#define SCLK	1
#define LATCH	7



/*******************************************/
/* Global share variables                  */
/*******************************************/

extern unsigned char LEDarray[3][STRING_SIZE];
extern unsigned char animation[11][3][8];
extern unsigned char face_animation[3][8];
extern unsigned char CharacterArray[26][5];

/*******************************************/
/* Function prototypes                     */
/*******************************************/
void Initialize(void);
void clear_array(void);
void SetupSPI(void);
void SendByteSPI(uint8_t data);
void SendRGBSPI(uint8_t RED, uint8_t GREEN, uint8_t BLUE);
void flipLatch(void);
unsigned char read_adc(uint8_t channel);
void SetupUSART(void);
unsigned char ReadByteUSART(void);
void SendByteUSART(char data);
void SendStringUSART(char *str);
void SetupMarqueeLEDarray(void);
void SetupCharacterArray(void);
void SetupAnimation(void);

#endif
