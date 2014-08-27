
// sensor_display.h

#include <util/delay.h>
#include <avr/io.h>
#include <stdlib.h>

//#include "adc.h"
#include "wunderproject.h"
#include "sensor_display.h"
#include "sensor_accel.h"
#include "sensor_buttons.h"
#include "sensor_statusleds.h"


unsigned char display_buf[DISPLAY_X_BUFSIZE][DISPLAY_COLOR_BUFSIZE] = {
						{0b00000000, 0b00000000, 0b00000000},       //column 0 : r,g,b
						{0b00000000, 0b00000000, 0b00000000}, 		//column 1 : r,g,b
						{0b00000000, 0b00000000, 0b00000000}, 		//column 2 : r,g,b
						{0b00000000, 0b00000000, 0b00000000}, 		//column 3 : r,g,b
						{0b00000000, 0b00000000, 0b00000000}, 		//column 4 : r,g,b
						{0b00000000, 0b00000000, 0b00000000}, 		//column 5 : r,g,b
						{0b00000000, 0b00000000, 0b00000000}, 		//column 6 : r,g,b
						{0b00000000, 0b00000000, 0b00000000} };		//column 7 : r,g,b

unsigned char color_draw_mode   = DISPLAY_COLOR_DRAW_MODE_REPLACE;

struct_cursor cursor = {
	.y                    = DISPLAY_Y_CENTER,
	.x                    = DISPLAY_X_CENTER,
	.ybits                = DISPLAY_COLOR_ALL_COLUMN_BITS_OFF,
	.cursor_blink_mask    = DISPLAY_CURSOR_BLINK_OFF,
	.cursor_blink_counter = 0
};


//void Sensor_Display_ClearBitmap(unsigned char *paint_color, unsigned char *display_buf)
void Sensor_Display_ClearBitmap()
{
	unsigned char x,c;

    // Step through each row
	for(x = DISPLAY_X_MIN; x <= DISPLAY_X_MAX; x++)
	{            	
	    // Fill the column bits for each row and each color
    	for(c = DISPLAY_COLOR_MIN; c <= DISPLAY_COLOR_MAX; c++)
    	{
    		// Fill column bits
    		display_buf[x][c] = DISPLAY_COLOR_ALL_COLUMN_BITS_OFF;
    		//if ( *(paint_color + c) ) displayBuf[x][c] = DISPLAY_COLOR_ALL_COLUMN_BITS_ON;
    		//else                      displayBuf[x][c] = DISPLAY_COLOR_ALL_COLUMN_BITS_OFF;
        }
    }
} // End : Sensor_Display_ClearBitmap()


void Sensor_Display_ToggleCursorBlink()
{
	if (cursor.cursor_blink_mask == DISPLAY_CURSOR_BLINK_OFF) cursor.cursor_blink_mask = DISPLAY_CURSOR_BLINK_RATE;
	else                                                      cursor.cursor_blink_mask = DISPLAY_CURSOR_BLINK_OFF;	
}


void Sensor_Display_ToggleCursorColorMode()
{
	// Toggle draw mode
	if (color_draw_mode == DISPLAY_COLOR_DRAW_MODE_REPLACE) color_draw_mode = DISPLAY_COLOR_DRAW_MODE_ADDITIVE;
	else                                                    color_draw_mode = DISPLAY_COLOR_DRAW_MODE_REPLACE; 

	// Update status LEDs
	Sensor_StatusLEDs_Update( (color_draw_mode != DISPLAY_COLOR_DRAW_MODE_REPLACE), STATUS_LED_7);
}

void Sensor_Display_UpdateCusor(struct_accel *accel_data)
{
	cursor.x = accel_data->accel_pos[ACCEL_X];
	cursor.y = accel_data->accel_pos[ACCEL_Y];

	// Set y axis bitpacked pixel bits
	cursor.ybits = 0x80 >> accel_data->accel_pos[ACCEL_Y];;

	// Update the cursor blink counter. It will auto-wrap around to zero
	cursor.cursor_blink_counter++;

}


void Sensor_Display_ClearPixelAtCursor()
{
	unsigned char c;
	for(c = 0; c < 3; c++)
	     display_buf[cursor.x][c] &=  ~cursor.ybits;
}

void Sensor_Display_DrawPixelAtCursor(struct_buttons *buttons)
{
	unsigned char c;

	// Enable drawing for colors which have their corresponding button pressed
	unsigned char rgb[3] = { (buttons->io_buttons & BTN_RHT_LT),
						     (buttons->io_buttons & BTN_RHT_UP),
							 (buttons->io_buttons & BTN_RHT_RT) };

	// If the current draw style is to replace the existing color
	// then clear the pixel before drawing on it
	if (color_draw_mode == DISPLAY_COLOR_DRAW_MODE_REPLACE)
	{		
		Sensor_Display_ClearPixelAtCursor();			        
	}

	// Set or clear color bit for current x,y cursor location
	for(c = 0; c < 3; c++)
	{
	    if ( rgb[c] ) { display_buf[cursor.x][c] |=  cursor.ybits; }
	}
}

void Sensor_Display_UpdateLEDArray()
{
	unsigned char x, c;

	// Update the LED array from the stored image	
    for(x = DISPLAY_X_MIN; x <= DISPLAY_X_MAX; x++)
	{           
		// If the cursor is set to blink this update
		// AND only if the column we are updating (x) matches the cursor x position
		if ((cursor.cursor_blink_counter & cursor.cursor_blink_mask) && (x == cursor.x))
		{ 		    		            		
			// Loop through the color bytes
        	for(c = 0; c < 3; c++)
        	{
        		if (cursor.cursor_blink_mask == DISPLAY_CURSOR_BLINK_OFF)
        		{
        			// Always invert if not blinking
            		SendByteSPI( display_buf[x][c] ^ cursor.ybits);
            	}
            	else
            	{
            		// If the pixel is set, blink dark (black), otherwise blink white
                	if (cursor.ybits & (display_buf[x][DISPLAY_COLOR_RED] | 
                		                display_buf[x][DISPLAY_COLOR_GRN] | 
                		                display_buf[x][DISPLAY_COLOR_BLU] ))
                	{
                		SendByteSPI( display_buf[x][c] & ~cursor.ybits);
                	}
                	else
                		SendByteSPI( display_buf[x][c] |  cursor.ybits);
                }
			}
        }
        else
        {            	
            //  All other columns get normal bitmap output
			for(c = 0; c < 3; c++)			
			 	SendByteSPI(display_buf[x][c]);
			 	
		} // end if : cursor blink check
		
		// Select the desired column of the LED array
		PORTE = (DISPLAY_X_MAX - x);
		
		flipLatch();
	} //end : for x
}
