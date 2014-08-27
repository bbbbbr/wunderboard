#ifndef sensor_accel_h
#define sensor_accel_h

// sensor_accel.h
//
// Device : MMA7331LCT-ND Accelerometer
//
// sensor.XOUT     -> MCU.ADC.6
// sensor.YOUT     -> MCU.ADC.5
// sensor.ZOUT     -> MCU.ADC.7
// sensor.G-SELECT <- MCU.PORT-D.4

struct struct_accel; // Forward declaration

typedef struct struct_accel {
	unsigned int  accel_zero_accum[3];     // Accumulates multiple zero reference point samples for averaging
	unsigned char accel_zero[3];           // Stores derived zero reference point values
	unsigned char accel_pos[3];            // Stores current position relative to origin

	unsigned char accel_zero_samples_left; // Trigger sensor zeroing on startup
	unsigned char accel_reset_smoothing;
} struct_accel;


void          Sensor_Accel_Init(struct_accel *);
void          Sensor_Accel_StartCalibration(struct_accel *);
unsigned char Sensor_Accel_CalibrationProcessing(struct_accel *);
void          Sensor_Accel_Update(struct_accel *);
unsigned char Sensor_Accel_ReadAndMapToDisplay(unsigned char, unsigned char, struct_accel *);
signed int    Sensor_Accel_Read(unsigned char, struct_accel *);


#define ACCEL_AVG_AMT 3

#define ACCEL_DISPLAY_CENTER 4
#define ACCEL_DISPLAY_MIN    0
#define ACCEL_DISPLAY_MAX    7

#define ACCEL_CALIBRATE_NUM_SAMPLES 10
#define ACCEL_CALIBRATE_READY        0 // No samples left, calibration complete


#define ACCEL_X 0
#define ACCEL_Y 1
#define ACCEL_Z 2

#define ACCEL_ADC_X 6 // ADC channel 5 is X axis
#define ACCEL_ADC_Y 5 // ADC channel 6 is Y axis
#define ACCEL_ADC_Z 7 // ADC channel 7 is Z axis

#endif