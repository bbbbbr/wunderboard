
// sensor_buttons.c

#include <util/delay.h>
#include <avr/io.h>
#include <stdlib.h>

//#include "adc.h"
#include "wunderproject.h"
#include "sensor_buttons.h"
#include "app.h"

void Sensor_Buttons_Init(struct_buttons *buttons)
{
    *buttons = (struct_buttons) {
        /* .io_buttons      = */ BTN_ALL_OFF,
        /* .io_buttons_last = */ BTN_ALL_OFF,
        /* .action_request  = */ ACTION_NONE
    };
}

void Sensor_Buttons_Update(struct_buttons *buttons)
{
    buttons->action_request = ACTION_NONE;

    // Read Buttons from Port A
    buttons->io_buttons = PINA;

    if (buttons->io_buttons != buttons->io_buttons_last) // TODO : remove
    {
        // Handle actions which will only have a single button pressed at once
        switch (buttons->io_buttons)
        {
            case BTN_LFT_UP : // Zero accelerometer request
                              buttons->action_request = ACTION_ACCEL_START_CALIBRATION;
                              break;

            case BTN_LFT_DN : // Clear display
                              buttons->action_request = ACTION_DISPLAY_CLEAR_BITMAP;
                              break;

            case BTN_LFT_LT : // Toggle cursor blinking
                              buttons->action_request = ACTION_CURSOR_TOGGLE_BLINK_MODE;
                              break;

            case BTN_LFT_RT : // Toggle color draw mode
                              buttons->action_request = ACTION_CURSOR_TOGGLE_COLOR_MODE;
                              break;

        }
    }

    // Handle actions which may have simulataneous buttons pressed
    // If erase button is pressed, clear to black
    if (buttons->io_buttons & BTN_RHT_DN)
    {
        // Clear current pixel
        buttons->action_request = ACTION_DISPLAY_CLEAR_CURSOR_PIXEL;

    }
    else if (buttons->io_buttons & (BTN_RHT_LT | BTN_RHT_UP | BTN_RHT_RT))
    {
        // Draw the current pixel
        buttons->action_request = ACTION_DISPLAY_DRAW_CURSOR_PIXEL;
    }

    buttons->io_buttons_last = buttons->io_buttons;

} // End : Sensor_Buttons_Update()
