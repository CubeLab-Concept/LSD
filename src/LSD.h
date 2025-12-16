#ifndef __LSD_H__
#define __LSD_H__

#include <Arduino.h>


class LSD {
    public:

        LSD(uint8_t data_pin, uint8_t clock_pin);

        // Color setter
        void set_color(uint8_t red, uint8_t green, uint8_t blue);
        void set_color(uint32_t hex);

    private:
        uint8_t _data_pin, _clock_pin;
        uint16_t _clock_speed = 20; // 20 ms clock speed

        // private methods
        void tick();
        void set_clock_speed(uint16_t clock_speed);
        void send_color(uint32_t col_data);
        void empty_frame();
};

#endif
