#ifndef I2C_SCAN_HPP_
#define I2C_SCAN_HPP_

#include <stdio.h>
#include <hardware/i2c.h>
#include <pico/stdlib.h>

/**
 * @brief Checks if the given address has special purpose, so we exclude
 * it from the scan
 * 
 * @param addr address to be checked
 * @return true if is special, otherwise false.
 */
bool reserved_addr(uint8_t addr) {
    return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}

/**
 * @brief Scans all the avaliable addresses and writes a map via the printf() to USB
 * of the addresses where devices are located.
 */
void i2c_scan(uint8_t PIN_SDA, uint8_t PIN_SCL){
    stdio_init_all();

    i2c_init(i2c0, 100 * 1000);
    gpio_set_function(PIN_SDA, GPIO_FUNC_I2C);
    gpio_set_function(PIN_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(PIN_SDA);
    gpio_pull_up(PIN_SCL);

    while (true) {
        printf("\nI2C Bus Scan for LCD and BPM280\n");
        printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");

        for (int addr = 0; addr < (1 << 7); ++addr) {
        if (addr % 16 == 0) {
            printf("%02x ", addr);
        }

        int ret;
        uint8_t rxdata;
        if (reserved_addr(addr))
            ret = PICO_ERROR_GENERIC;
        else
            ret = i2c_read_blocking(i2c0, addr, &rxdata, 1, false);

        printf(ret < 0 ? "." : "@");
        printf(addr % 16 == 15 ? "\n" : "  ");
        }

        sleep_ms(2000);
    }
}

#endif