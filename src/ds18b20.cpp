#include "sensors/ds18b20.h"
#include "pico/stdlib.h"

DS18B20::DS18B20(uint pin) : gpio_pin(pin) {
    gpio_init(gpio_pin);
    gpio_pull_up(gpio_pin);  // Enable pull-up resistor
}

void DS18B20::delay_us(uint32_t us) {
    sleep_us(us);
}

bool DS18B20::reset() {
    gpio_set_dir(gpio_pin, GPIO_OUT);
    gpio_put(gpio_pin, 0);
    delay_us(480);  // Pull low for 480µs

    gpio_set_dir(gpio_pin, GPIO_IN);
    delay_us(70);  // Release and wait 70µs

    bool presence = !gpio_get(gpio_pin);  // Check for presence pulse

    delay_us(410);  // Wait for the rest of the timeslot
    return presence;
}

void DS18B20::write_bit(bool bit) {
    gpio_set_dir(gpio_pin, GPIO_OUT);
    gpio_put(gpio_pin, 0);
    delay_us(bit ? 6 : 60);
    gpio_put(gpio_pin, 1);
    delay_us(bit ? 64 : 10);
}

bool DS18B20::read_bit() {
    gpio_set_dir(gpio_pin, GPIO_OUT);
    gpio_put(gpio_pin, 0);
    delay_us(6);
    gpio_set_dir(gpio_pin, GPIO_IN);
    delay_us(9);

    bool bit = gpio_get(gpio_pin);
    delay_us(55);
    return bit;
}

void DS18B20::write_byte(uint8_t byte) {
    for (int i = 0; i < 8; i++) {
        write_bit(byte & 0x01);
        byte >>= 1;
    }
}

uint8_t DS18B20::read_byte() {
    uint8_t byte = 0;
    for (int i = 0; i < 8; i++) {
        if (read_bit()) {
            byte |= (1 << i);
        }
    }
    return byte;
}

bool DS18B20::initialize() {
    return reset();
}

float DS18B20::read_temperature() {
    if (!reset()) {
        return -999.0f;  // Error value if sensor not detected
    }

    write_byte(0xCC);  // Skip ROM
    write_byte(0x44);  // Start temperature conversion

    sleep_ms(750);  // Wait for conversion to complete

    reset();
    write_byte(0xCC);  // Skip ROM
    write_byte(0xBE);  // Read Scratchpad

    uint8_t temp_lsb = read_byte();
    uint8_t temp_msb = read_byte();

    int16_t temp = (temp_msb << 8) | temp_lsb;
    return temp * 0.0625f;  // Convert to Celsius
}
