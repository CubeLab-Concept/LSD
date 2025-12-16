#include <Arduino.h>
#include "LSD.h"


LSD::LSD(uint8_t data_pin, uint8_t clock_pin):
         _data_pin(data_pin),
         _clock_pin(clock_pin)
{
    /* LSD class constructor*/

    // Define data and clock pins as outputs
    pinMode(_data_pin, OUTPUT);
    pinMode(_clock_pin, OUTPUT);
}


void LSD::set_color(uint8_t red, uint8_t green, uint8_t blue) {
    /* Set the LED color to the RGB values in parameters (between 0-255) */
    
    uint32_t col_data = 0;

    // P9813 frame formatting
    col_data |= (uint32_t)0x03 << 30;       // first 2 bits are 11

    col_data |= (uint32_t)(blue>>6) << 28;  // Color verify bits are 
    col_data |= (uint32_t)(green>>6) << 26; // first two MSB for each color
    col_data |= (uint32_t)(red>>6) << 24;   // ordered BLUE/GREEN/RED

    col_data |= (uint32_t)blue << 16;      // Raw color data
    col_data |= (uint32_t)green << 8;      // 8 bit for each color
    col_data |= red;                       // MSB first, same order

    // Send color data to IC
    send_color(col_data);
}


void LSD::set_color(uint32_t hex_color) {
    /* Set color data from hex value */
    uint8_t red   = (hex_color >> 16) & 0xFF;
    uint8_t green = (hex_color >> 8)  & 0xFF;
    uint8_t blue  =  hex_color        & 0xFF;
    setColor(red, green, blue);
}


void LSD::send_color(uint32_t col_data) {
    /* Send the color data to the P9813 IC */
    
    // Empty frame as separator
    empty_frame();
    uint32_t bit_index;

    // Sendata_ping color data bits, MSB first
    for (uint8_t i = 0; i < 32; i++) {

        // mask the value for each data bit
        bit_index = 0x80000000 >> i; 

        // Digital write either HIGH or LOW dependata_ping on the masked bit
        digitalWrite(this->_data_pin, ((col_data & bit_index) != 0) ? HIGH : LOW);

        // Tick once
        tick();
    }
    // Empty frame to end transmission
    empty_frame();
}


void LSD::empty_frame() {
    /* Send an empty frame */
    
    // set data pin low
    digitalWrite(this->_data_pin, LOW);

    // Then tick for a full frame (32 bits)
    for (uint8_t i = 0; i < 32; i++) {
        tick();
    }
}


void LSD::tick() {
    /* Make the clock tick once */
    digitalWrite(this->_clock_pin, LOW);
    delayMicroseconds(this->_clock_speed);
    digitalWrite(this->_clock_pin, HIGH);
    delayMicroseconds(this->_clock_speed);
}


void LSD::set_clock_speed(uint16_t clock_speed){
    /*Change the clock speed for data transmission*/
    this->_clock_speed = clock_speed;
}