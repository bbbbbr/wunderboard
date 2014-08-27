/***************
** Author: Chelsea Collette
** Company: TekBots
** Date: 08/14/2013
** Input: None
** Output: LED Array
** Description: Displays the color spectrum currently allowed on the Wunderboard at a diagonal.
***************/

#include <util/delay.h>
#include <avr/io.h>
#include <stdlib.h>

//#include "adc.h"
#include "wunderproject.h"

#include "sensor_accel.h"
#include "sensor_buttons.h"
#include "sensor_display.h"
#include "app.h"

// 9600 baud
#define BAUD_RATE 51

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))

	
void main(void) {
	Initialize(); //calls a function to initialize the chip
	SetupSPI(); //sets up SPI 


	struct_buttons buttons;
	struct_accel   accel_data;

	Sensor_Buttons_Init( &buttons );
	Sensor_Accel_Init( &accel_data );

	while(1)
	{
		// Read the Accelerometer and use it to update the cursor
		Sensor_Accel_Update( &accel_data );

		Sensor_Display_UpdateCusor( &accel_data );

	    Sensor_Buttons_Update( &buttons );

		switch( buttons.action_request )
		{
			case ACTION_ACCEL_START_CALIBRATION    : Sensor_Accel_StartCalibration( &accel_data ); 
											         break;

			case ACTION_DISPLAY_CLEAR_BITMAP       : Sensor_Display_ClearBitmap(); 
											         break;

			case ACTION_CURSOR_TOGGLE_BLINK_MODE   : Sensor_Display_ToggleCursorBlink(); 
											         break;

			case ACTION_CURSOR_TOGGLE_COLOR_MODE   : Sensor_Display_ToggleCursorColorMode(); 
											         break;

			case ACTION_DISPLAY_CLEAR_CURSOR_PIXEL : Sensor_Display_ClearPixelAtCursor();
											         break;

			case ACTION_DISPLAY_DRAW_CURSOR_PIXEL  : Sensor_Display_DrawPixelAtCursor( &buttons );
											         break;
		}

		Sensor_Display_UpdateLEDArray();

	} //end infinite while loop

} //end main
	
	
	