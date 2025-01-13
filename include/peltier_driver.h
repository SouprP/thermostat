#ifndef PELTIER_DRIVER_H_
#define PELTIER_DRIVER_H_

#define PICO_MUTEX_ENABLE_SDK120_COMPATIBILITY 1

#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include <pico/multicore.h>
#include <pico/mutex.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <sensors/ds18b20.h>
#include "hardware/sync.h"
#include <pico/stdlib.h>

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
        // bool isWorking;
        bool isCooling;
        bool isHeating;

        // mutex for sync
        static mutex_t peltier_mutex;

    public:
        // PeltierDriver();
        // ~PeltierDriver();

        void init(uint8_t heat_gpio, uint8_t cold_gpio, DS18B20 *ds18b20);
        void set_target_temp(float target_temp);
        float get_target_temp();
        float get_temp();
        void drive();
};

#endif