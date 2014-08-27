#ifndef sensor_display_h
#define sensor_display_h

// sensor_display.h
// 
// Display is accessed through Port E
//
// Addressing :
// +----------------+
// |                |
// |     Display    |
// |                |
// |----------------|
// |                |
// |     Buttons    |
// |                |
// +----------------+
//
// Logical layout (abstracted) :
//
//   7/left --------> 0/right : Columns : write to Port E 
//   |
//   | 
//   |
//   \|/   
//   0    Rows : top = 7 = msbit, bottom = 0 = lsbit
//
// X Axis :
//   * Bit packed column data is written in 3 bytes at a time, one byte per color channel
//   * Addressed color-channel auto-increments via shift register
//   * Addressed column is set by writing value to to Port E and then "latching"
//   * Column latching is performed by toggling Port B.7 high, delaying 1ms, then low
//   * Right-most column is written "first"
//   * Left-most column gets written "last"
// 
// Y Axis : 
//   * Stored as bitpacked pixels, one bit per pixel - per color channel
//   * One byte stores entire column of pixel data
//   * msbit = top
//   * lsbit = bottom

#include "sensor_buttons.h"
#include "sensor_accel.h"

struct struct_cursor; // Forward declaration

typedef struct struct_cursor {
	unsigned char y;
	unsigned char x;
	unsigned char ybits;
	unsigned char cursor_blink_mask;
	unsigned char cursor_blink_counter;
} struct_cursor;

void Sensor_Display_ClearBitmap(void);
void Sensor_Display_ToggleCursorBlink(void);
void Sensor_Display_ToggleCursorColorMode(void);
void Sensor_Display_UpdateCusor(struct_accel *);
void Sensor_Display_ClearPixelAtCursor(void);
void Sensor_Display_DrawPixelAtCursor(struct_buttons *);
void Sensor_Display_UpdateLEDArray(void);

#define DISPLAY_X_BUFSIZE     8
#define DISPLAY_COLOR_BUFSIZE 3

#define DISPLAY_Y_MIN     0
#define DISPLAY_Y_CENTER  3
#define DISPLAY_Y_MAX     7
#define DISPLAY_X_MIN     0
#define DISPLAY_X_CENTER  3
#define DISPLAY_X_MAX     7

#define DISPLAY_COLOR_MIN 0
#define DISPLAY_COLOR_MAX 2

#define DISPLAY_COLOR_RED 0
#define DISPLAY_COLOR_GRN 1
#define DISPLAY_COLOR_BLU 2

#define DISPLAY_COLOR_ALL_COLUMN_BITS_OFF 0x00
#define DISPLAY_COLOR_ALL_COLUMN_BITS_ON  0xFF

#define DISPLAY_COLOR_DRAW_MODE_ADDITIVE 0
#define DISPLAY_COLOR_DRAW_MODE_REPLACE  1

#define DISPLAY_CURSOR_BLINK_OFF  0xFF // 0x20
#define DISPLAY_CURSOR_BLINK_RATE 0x20

#endif
