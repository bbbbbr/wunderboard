
// sensor_accel.c

#include <util/delay.h>
#include <avr/io.h>
#include <stdlib.h>

//#include "adc.h"
#ifndef WUNDERPROJECT_H
  #include "wunderproject.h"
#endif

#include "sensor_accel.h"

void Sensor_Accel_Init(struct_accel *accel_data)
{
    *accel_data = (struct_accel) {
        /*. accel_zero_accum[3]     = */ {0,0,0},
        /*. accel_zero[3]           = */ {0,0,0},
        /*. accel_pos[3]            = */ {ACCEL_DISPLAY_CENTER, ACCEL_DISPLAY_CENTER, ACCEL_DISPLAY_CENTER}, // X,Y,Z

        /*. accel_zero_samples_left = */ ACCEL_CALIBRATE_NUM_SAMPLES,
        /*. accel_reset_smoothing   = */ FALSE
    };
}


void Sensor_Accel_StartCalibration(struct_accel *accel_data)
{
    accel_data->accel_zero_accum[ACCEL_X] = 0;
    accel_data->accel_zero_accum[ACCEL_Y] = 0;
    accel_data->accel_zero_accum[ACCEL_Z] = 0;

    accel_data->accel_zero_samples_left = ACCEL_CALIBRATE_NUM_SAMPLES;
}


unsigned char Sensor_Accel_CalibrationProcessing(struct_accel *accel_data)
{
    // Update zero reference point accumulation samples if requested
    if (accel_data->accel_zero_samples_left)
    {
        // Store a new sample for each axis
        accel_data->accel_zero_accum[ACCEL_X] += read_adc(ACCEL_ADC_X);
        accel_data->accel_zero_accum[ACCEL_Y] += read_adc(ACCEL_ADC_Y);
        accel_data->accel_zero_accum[ACCEL_Z] += read_adc(ACCEL_ADC_Z);

        accel_data->accel_zero_samples_left--;

        // If it's the last sample then process the accumulated data
        if (accel_data->accel_zero_samples_left == ACCEL_CALIBRATE_READY)
        {
            // Average the samples to get a more stable value and store the results for use
            accel_data->accel_zero[ACCEL_X] = accel_data->accel_zero_accum[ACCEL_X] / ACCEL_CALIBRATE_NUM_SAMPLES;
            accel_data->accel_zero[ACCEL_Y] = accel_data->accel_zero_accum[ACCEL_Y] / ACCEL_CALIBRATE_NUM_SAMPLES;
            accel_data->accel_zero[ACCEL_Z] = accel_data->accel_zero_accum[ACCEL_Z] / ACCEL_CALIBRATE_NUM_SAMPLES;

            // Signal that averaging of processed data should be reset on the next update
            accel_data->accel_reset_smoothing = TRUE;
        }
    }

    return (accel_data->accel_zero_samples_left);

} // End : Sensor_Accel_CalibrationProcessing()



void Sensor_Accel_Update(struct_accel *accel_data)
{
    // Handle zero ref calibration if needed
    if (Sensor_Accel_CalibrationProcessing( accel_data ) == ACCEL_CALIBRATE_READY)
    {
        accel_data->accel_pos[ACCEL_X] = Sensor_Accel_ReadAndMapToDisplay( accel_data->accel_pos[ACCEL_X], ACCEL_X, accel_data);
        accel_data->accel_pos[ACCEL_Y] = Sensor_Accel_ReadAndMapToDisplay( accel_data->accel_pos[ACCEL_Y], ACCEL_Y, accel_data);

    if (accel_data->accel_reset_smoothing) accel_data->accel_reset_smoothing = FALSE; // clear flag now that it's been processed

    }
}


unsigned char Sensor_Accel_ReadAndMapToDisplay(unsigned char accel_prev_data, unsigned char accel_axis, struct_accel *accel_data)
{
    signed int accel_new_data;

    // Read sensor and offset position to be oriented around display center
    accel_new_data = ACCEL_DISPLAY_CENTER + Sensor_Accel_Read(accel_axis, accel_data);

    // Average/smooth new data into existing data (if reset not requested)
    if (! accel_data->accel_reset_smoothing )
        accel_new_data = (((signed int)accel_prev_data * ACCEL_AVG_AMT) + accel_new_data) / (ACCEL_AVG_AMT + 1);

    // Bounds check to keep position within display range
    if      (accel_new_data < ACCEL_DISPLAY_MIN) accel_new_data = ACCEL_DISPLAY_MIN;
    else if (accel_new_data > ACCEL_DISPLAY_MAX) accel_new_data = ACCEL_DISPLAY_MAX;

    // Downgrade data to unsigned char now that it's mapped to the display
    return (unsigned char)accel_new_data;
}


signed int Sensor_Accel_Read(unsigned char accel_axis, struct_accel *accel_data)
{
    signed int    accel_new_data    = 0;
    unsigned char accel_adc_channel = 0;

    // Select ADC channel for the given axis
    switch(accel_axis)
    {
        case ACCEL_X : accel_adc_channel = ACCEL_ADC_X; break;
        case ACCEL_Y : accel_adc_channel = ACCEL_ADC_Y; break;
        case ACCEL_Z : accel_adc_channel = ACCEL_ADC_Z; break;
    }

    // Read the ADC and apply the axis zero reference offset
    if (accel_adc_channel)
        accel_new_data = ((signed int)read_adc(accel_adc_channel) - (signed int)accel_data->accel_zero[accel_axis]);

    return (accel_new_data);
}
