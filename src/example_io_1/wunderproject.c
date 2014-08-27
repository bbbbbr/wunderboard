

//Includes
#include <avr/io.h>
#include "wunderproject.h"
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

//Global variables
unsigned char LEDarray[3][STRING_SIZE];
unsigned char animation[11][3][8];
unsigned char face_animation[3][8];
unsigned char CharacterArray[26][5];

//Initialize PORT data direction register
void Initialize(){
    //1 = output, 0 = input
    DDRA = 0x00; // Port A (PINA) pins read from buttons

    // Port B pins are output for SPI (TODO : why did they set both MISO and MOSI to output?)
    //set spi pins to output, refer to #defines for the pin numbers
    DDRB = (1<<MISO) | (1<<MOSI) | (1<<SCLK) | (1<<LATCH);;

    DDRC = 0xff; // Port C pins output to single LEDs
    //DDRD = 0x08;
    DDRD = 0x18; // Port D.4 = G select for accelerometer

    DDRE = 0x07; // Port E pins [2..0] output to LED array column select
    DDRF = 0x00;
    return;
}

/** The clearArray() function turns off all LEDS on the Wunderboard array. It accepts no inputs and returns nothing*/
void clear_array(void){
    int j;
    SetupSPI();

    PORTC = 0x00; //turns off SMT LEDs on PCB

    for(j = 1; j <= 24; j++){ //24 for 8 columns with 3 colors
        //Start transmission
        SPDR = 0x00; //sends all 0s over SPI to turn off all LEDs
        //Wait for transmission complete
        while (!(SPSR & (1<<SPIF)));

        if((j % 3) == 0){
            PORTE = (j - 1) / 3; //determines the column

            PORTB|=1<<7; //latch (explain how it works)
            _delay_ms(1);
            PORTB&=~(1<<7);
        } //end if statement

    } //end for loop

} //end clear_array function

/**********************************************************/
/* SPI functions                                          */
/**********************************************************/
//Enable SPI in master mode
void SetupSPI(){
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
    return;
}

//Send a byte via SPI
void SendByteSPI(uint8_t data){
    SPDR = data;
    while(!(SPSR & (1<<SPIF))){}
    return;
}

//Send 3 bytes for RGB and flip latch to show color
void SendRGBSPI(uint8_t RED, uint8_t GREEN, uint8_t BLUE){
    SendByteSPI(RED);
    SendByteSPI(GREEN);
    SendByteSPI(BLUE);
    flipLatch();
    return;
}

//flip LED latch to load spi data to color
void flipLatch(){
    PORTB |= (1<<LATCH);
    _delay_ms(1);
    PORTB &= ~(1<<LATCH);
    return;
}


/**********************************************************/
/* ADC functions                                          */
/**********************************************************/
//read ADC channel
//channel 5 - Y axis
//channel 6 - X axis
//channel 7 - Z axis
unsigned char read_adc(uint8_t channel){
    unsigned char test;

    ADMUX = 0x60 | channel;         // Set the channel to the one we want
    ADCSRA = 0b11000110;            // Start a new sample.
    while ((ADCSRA & 0x10) == 0 );  // Wait for a Valid Sample
    ADCSRA |= 0b00010000;           // Tell ADC you have the sample you want.
    ADCSRA |= 0b01000000;           // Start a new sample.
    while ((ADCSRA & 0x10) == 0 );  // Wait for a Valid Sample
    ADCSRA |= 0b00010000;           // Tell ADC you have the sample you want.

    test = ADCH;
    ADCSRA = 0x00;                  // Disable the ADC

    return (test);
}


/**********************************************************/
/* USART functions                                        */
/**********************************************************/
//Enable USART BD = 9600, 8bit, no parity, 1SB
void SetupUSART(){
    UCSR1A = (1<<U2X1);                 //Enable double speed
    UCSR1B = (1<<RXEN1) | (1<<TXEN1);   //Enable RX & TX
    UCSR1C = (3<<UCSZ10);               //8 bit, no parity, 1 SB
    UBRR1 = 12;                         //9600
    return;
}

//read USART buffer and return the data
unsigned char ReadByteUSART(){
    while(! (UCSR1A & (1<<RXC1))){}     //wait for data
    return UDR1;                        //return data
}

//Send byte via USART
void SendByteUSART(char data){
    while(!(UCSR1A & (1<<UDRE1))){}     //wait for register to be ready
    UDR1 = data;                        //load data to tx buffer
}

//Send a string of ascii
void SendStringUSART(char *str){
    while(*str){                //continue till string is empty
        SendByteUSART(*str);    //Send byte
        str++;                  //move to next byte
    }
    return;
}
