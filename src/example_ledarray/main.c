/***************
** Author: Chelsea Collette
** Company: TekBots
** Date: 08/14/2013
** Input: None
** Output: LED Array
** Description: Displays the color spectrum currently allowed on the Wunderboard at a diagonal.
***************/

#include <avr/delay.h>
#include <avr/io.h>
#include <stdlib.h>

#include "wunderproject.h"

// 9600 baud
#define BAUD_RATE 51

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))

	
void main(){
	int j, k;
	Initialize(); //calls a function to initialize the chip
	SetupSPI(); //sets up SPI 
	clear_array(); //turns off all LEDs
	
	//Following is the image to be displayed. Columns by RGB
	unsigned char colors[8][3] = { 
		{0b11110000, 0b00111111, 0b00000011},       //column 0
		{0b11110000, 0b00111111, 0b00000011}, 		//column 1
		{0b11111100, 0b00001111, 0b11000000}, 		//column 2
		{0b11111100, 0b00001111, 0b11000000}, 		//column 3
		{0b00111111, 0b00000011, 0b11110000}, 		//column 4
		{0b00111111, 0b00000011, 0b11110000}, 		//column 5
		{0b00001111, 0b11000000, 0b11111100}, 		//column 6
		{0b00001111, 0b11000000, 0b11111100} };		//column 7
	
	while(1){
	
		for(j = 0; j < 8; j++){ //transmit colors for each column
			for(k = 0; k < 3; k++){ //transmit each color
				SendByteSPI(colors[j][k]); //send color over SPI
			} //end of k
			
			PORTE = j; //the column of the  LED array
			
			flipLatch();
		} //end of j
		

	} //end while

} //end main
	
	
	