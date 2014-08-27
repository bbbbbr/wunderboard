
// sensor_statusleds.c

#include <util/delay.h>
#include <avr/io.h>
#include <stdlib.h>

//#include "adc.h"
#include "wunderproject.h"
#include "sensor_statusleds.h"
#include "app.h"


void Sensor_StatusLEDs_Update(unsigned char status_enable, unsigned char status_mask)
{
    // Clear bits indicated by mask
    PORTC &= ~status_mask;

    // Set bits if requested bits
    if (status_enable)
        PORTC |= status_mask;
}
