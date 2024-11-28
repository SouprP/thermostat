#ifndef DS18B20_HPP
#define DS18B20_HPP

#include "pico/stdlib.h"

class DS18B20 {
private:
    uint gpio_pin;  // GPIO pin for DS18B20 data line
    
    void delay_us(uint32_t us);     // Microsecond delay
    bool reset();                   // Reset and check presence
    void write_bit(bool bit);       // Write a single bit
    bool read_bit();                // Read a single bit
    void write_byte(uint8_t byte);  // Write a byte
    uint8_t read_byte();            // Read a byte

public:
    DS18B20(uint pin); 
    bool init(); 
    float read_temperature(); 
};

#endif
