#ifndef sensor_statusleds_h
#define sensor_statusleds_h

// sensor_statusleds.h
//
// Status LEDs are on Port C (active high)
//
// +----------------+
// |                |
// |     Display    |
// |                |
// |----------------|
// |  .7 .... .0    |
// |                |
// +----------------+

void Sensor_StatusLEDs_Update(unsigned char, unsigned char);

#define STATUS_LED_7 0b10000000 // Port C.7
#define STATUS_LED_6 0b01000000 // Port C.6
#define STATUS_LED_5 0b00100000 // Port C.5
#define STATUS_LED_4 0b00010000 // Port C.4
#define STATUS_LED_3 0b00001000 // Port C.3
#define STATUS_LED_2 0b00000100 // Port C.2
#define STATUS_LED_1 0b00000010 // Port C.2
#define STATUS_LED_0 0b00000001 // Port C.0

#endif
