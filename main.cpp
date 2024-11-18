#include <stdio.h>
#include "pico/stdlib.h"
#include <pico/rand.h>
#include <hardware/i2c.h>

#include <i2c_scan.hpp>
#include <sensors/ds18b20.h>

#define LCD_ADDR 0x27
#define BMP_ADDR 0x76
#define SDA_PIN 4
#define SCL_PIN 5



int main() {
    stdio_init_all();
    DS18B20 sensor(2); 

    while (true) {
        if (sensor.initialize()) {
            float temperature = sensor.read_temperature();
            printf("Temperature: %.2f°C\n", temperature);
        } else {
            printf("DS18B20 not detected\n");
        }

        sleep_ms(1000);
    }
}

