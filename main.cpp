#include <stdio.h>
#include "pico/stdlib.h"
#include <pico/rand.h>
#include <hardware/i2c.h>

#include <i2c_scan.hpp>
#include <sensors/ds18b20.h>
#include <sensors/bme680.h>
#include <peltier_driver.h>

#define LCD_ADDR        0x27
#define BMP_ADDR        0x76
#define BME680_ADDR     0x77
#define SDA_PIN         4
#define SCL_PIN         5



int main() {
    stdio_init_all();
    //DS18B20 *sensor = new DS18B20(2); 
    // BME680 bme680;

    PeltierDriver peltier;
    peltier.init(3, 4, nullptr);
    peltier.drive();

    // i2c_inst_t* i2c = i2c0;

    // Initialize I2C once
    // i2c_init(i2c, 100 * 1000);

    // bme680.init(i2c, BME680_ADDR);
    // bme680.set_pressure_oversampling(bme680_oversampling_rate_t::osr_16x);
    // i2c_scan(SDA_PIN, SCL_PIN);

    // while (true) {
    //     if (sensor->init()) {
    //         float temperature = sensor->read_temperature();
    //         printf("Temperature: %.2f°C\n", temperature);
    //     } else {
    //         printf("DS18B20 not detected\n");
    //     }

    //     sleep_ms(1000);
    // }
}

