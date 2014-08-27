#ifndef sensor_buttons_h
#define sensor_buttons_h

// sensor_buttons.h
//
// Buttons are on Port A, active high (pressed = 1)
//
// +----------------+
// |                |
// |     Display    |
// |                |
// |----------------|
// |  .6  + +  .2   |
// |.7  .4   .3  .0 |
// |  .5       .1   |
// +----------------+

// OPTIONAL : Could use debouncing

struct struct_buttons; // Forward declaration

typedef struct struct_buttons {
    unsigned char io_buttons;
    unsigned char io_buttons_last;
    unsigned char action_request;
} struct_buttons;

void Sensor_Buttons_Init(struct_buttons *);
void Sensor_Buttons_Update(struct_buttons *);

#define BTN_LFT_UP 0b01000000 // PortA.6
#define BTN_LFT_DN 0b00100000 // PortA.5
#define BTN_LFT_LT 0b10000000 // PortA.7
#define BTN_LFT_RT 0b00010000 // PortA.4

#define BTN_RHT_UP 0b00000100 // PortA.2
#define BTN_RHT_DN 0b00000010 // PortA.1
#define BTN_RHT_LT 0b00001000 // PortA.3
#define BTN_RHT_RT 0b00000001 // PortA.0

#define BTN_ALL_OFF 0b00000000 // PortA

#endif
