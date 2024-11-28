#ifndef PELTIER_DRIVER_H_
#define PELTIER_DRIVER_H_

#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include <stdio.h>

#include <sensors/ds18b20.h>

class PeltierDriver{
    private:
        // GPIOs for controlling the H bridge
        uint8_t HEAT_GPIO;
        uint8_t COLD_GPIO;

        // temperature sensor attachated to the peltier module
        DS18B20 *ds18b20;

        // target temperature for debugging
        float target_temp = 35.0f;
        float offset = 2.0f;

        // is heating up or cooling down
        bool isWorking;

    public:
        // PeltierDriver();
        // ~PeltierDriver();

        void init(uint8_t heat_gpio, uint8_t cold_gpio, DS18B20 *ds18b20);
        void drive();
};

#endif