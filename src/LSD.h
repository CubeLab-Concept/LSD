#ifndef __LSD_H__
#define __LSD_H__

#include <Arduino.h>


class LSD {
    public:

        LSD(uint8_t data_pin, uint8_t clock_pin);
        LSD(uint8_t data_pin, uint8_t clock_pin, uint8_t nb_module);

        // Color setter
        void set_color(uint8_t red, uint8_t green, uint8_t blue, uint8_t module_id, bool doShow);
        void set_color(uint8_t red, uint8_t green, uint8_t blue, uint8_t module_id);
        void set_color(uint32_t hex, uint8_t module_id, bool doShow);
        void set_color(uint32_t hex, uint8_t module_id);
        
        void set_color(uint8_t red, uint8_t green, uint8_t blue, bool doShow);
        void set_color(uint8_t red, uint8_t green, uint8_t blue);
        void set_color(uint32_t hex, bool doShow);
        void set_color(uint32_t hex);
        
        void send_colors();

    private:
        // Data and clock pins
        uint8_t _data_pin, _clock_pin;
        uint8_t _nb_modules; // Set number of PC9813 (16 max)
        unsigned _clock_speed = 1; // 1 ms = 1kHz clock
        
        // PC9813 formated color data, 16 modules max
        uint32_t _led_colors[16]; 

        // private methods
        void tick();
        void set_clock_speed(uint16_t clock_speed);
        uint32_t format_color(uint8_t red, uint8_t green, uint8_t blue);
        void send_frame(uint32_t frame);
        void empty_frame();
};

#endif
