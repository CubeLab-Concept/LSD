#include <Arduino.h>
#include "LSD.h"


LSD::LSD(uint8_t data_pin, uint8_t clock_pin, uint8_t nb_module):
_data_pin(data_pin), _clock_pin(clock_pin), _nb_modules(nb_module)
{    /* LSD class constructor*/

    
    // Define data and clock pins as outputs
    pinMode(this->_data_pin, OUTPUT);
    pinMode(this->_clock_pin, OUTPUT);

}

LSD::LSD(uint8_t data_pin, uint8_t clock_pin)
{
    /* Constructeur pour un unique module P9813 */
    LSD(data_pin, clock_pin, 1);
}

uint32_t LSD::format_color(uint8_t red, uint8_t green, uint8_t blue){
    /* Format a color to PS9813 data format */
    uint32_t col_data = 0;

    // P9813 frame formatting
    col_data |= (uint32_t)0x03 << 30;       // first 2 bits are 11

    col_data |= (uint32_t)(blue>>6) << 28;  // Color verify bits are 
    col_data |= (uint32_t)(green>>6) << 26; // first two MSB for each color
    col_data |= (uint32_t)(red>>6) << 24;   // ordered BLUE/GREEN/RED

    col_data |= (uint32_t)blue << 16;      // Raw color data
    col_data |= (uint32_t)green << 8;      // 8 bit for each color
    col_data |= red;                       // MSB first, same order

    return(col_data);
}


////////////////////////////////////////////////////////////////////////////
// set_colors definition and overloads
//
// Single module setters
//
void LSD::set_color(uint8_t red, uint8_t green, uint8_t blue, uint8_t module_id, bool doShow){
    /* Set the LED color to the RGB values in parameters (between 0-255)
    The module ID can be set.
    Data is only sent immediatly if doShow is true
    */
    
    uint32_t col_data;

    // Compute color
    col_data = this->format_color(red, green, blue);
    this->_led_colors[module_id] = col_data;    

    // Send color data to IC?
    if(doShow){
        send_colors();
    }
}


void LSD::set_color(uint8_t red, uint8_t green, uint8_t blue, uint8_t module_id){
    /*Apply RGB color to module_id module and display it*/
    set_color(red,green, blue, module_id, true);
}


void LSD::set_color(uint32_t hex_color, uint8_t module_id, bool doShow){
    /* Set color data from hex value to specific moudule. Only show if required*/
    uint8_t red   = (hex_color >> 16) & 0xFF;
    uint8_t green = (hex_color >> 8)  & 0xFF;
    uint8_t blue  =  hex_color        & 0xFF;
    set_color(red, green, blue, module_id, doShow);
}

void LSD::set_color(uint32_t hex_color, uint8_t module_id){
    /* Set color data from hex value to specific moudule. Always apply immediatly*/
    set_color(hex_color, module_id, true);
}

////////////////////////////////////////////////////////////////////////////
// All module setters
//
void LSD::set_color(uint8_t red, uint8_t green, uint8_t blue, bool doShow){
    /* Apply colors to all modules */
    for(int i = 0; i< this->_nb_modules; i++){
        set_color(red, green, blue, i, false);
    }

    // Send color data to IC?
    if(doShow){
        send_colors();
    }
}
void LSD::set_color(uint8_t red, uint8_t green, uint8_t blue){
    set_color(red, green, blue, true);
}

void LSD::set_color(uint32_t hex_color, bool doShow){
    /*Apply hex colors to all modules, show if it is set*/
    for(int i = 0; i< this->_nb_modules; i++){
        set_color(hex_color, i, false);
    }

    // Send color data to IC?
    if(doShow){
        send_colors();
    }
}

void LSD::set_color(uint32_t hex_color) {
    /*Apply hex colors to all modules and immediatly send data to IC*/
    set_color(hex_color, true);
}

//
//  End of set_colors
///////////////////////////////////////////////////////////////////////////////

void LSD::send_colors() {
    /* Send the colors data to the P9813 IC */
    
    // Empty frame as separator
    empty_frame();

    // Transmet l'ensemble des données LED aux P9813
    for(int i =0; i< this->_nb_modules; i++){
        send_frame(this->_led_colors[i]);
    }
    // Empty frame to end transmission
    empty_frame();
}

void LSD::send_frame(uint32_t frame){
    /* Transmer un bloc de 32bit de données au P9813*/
    uint32_t bit_index;

    // Sendata_ping color data bits, MSB first
    for (uint8_t i = 0; i < 32; i++) {

        // mask the value for each data bit
        bit_index = 0x80000000 >> i; 

        // Digital write either HIGH or LOW dependata_ping on the masked bit
        digitalWrite(this->_data_pin, ((frame & bit_index) != 0) ? HIGH : LOW);

        // Tick once
        tick();
    }
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