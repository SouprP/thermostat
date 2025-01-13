#include <peltier_driver.h>


void PeltierDriver::init(uint8_t heat_gpio, uint8_t cold_gpio, DS18B20 *ds18b20){
    // this->ds18b20 = ds18b20;
    this->ds18b20 = new DS18B20(9);
    this->HEAT_GPIO = heat_gpio;
    this->COLD_GPIO = cold_gpio;

    // this->isWorking = false;
    this->isHeating = false;
    this->isCooling = false;

    gpio_init(HEAT_GPIO);
    gpio_init(COLD_GPIO);

    gpio_set_dir(HEAT_GPIO, GPIO_OUT);
    gpio_set_dir(COLD_GPIO, GPIO_OUT);

    gpio_put(HEAT_GPIO, 1);
    gpio_put(COLD_GPIO, 1);

    ds18b20->init();
    srand(time(NULL));
}

void PeltierDriver::drive(){
    while (true) {
        sleep_ms(1000);
        float temp = ds18b20->read_temperature();
        //printf("Current temperature: %.2f°C -> %.2f°C\n", temp, target_temp);

        // if(!isCooling && !isHeating){
        //     target_temp = rand() % 36 + 15; // new temp between 20 and 35
        //     printf("No active heating/cooling. New target temperature: %.2f°C\n", target_temp);
        // }

        if (temp < target_temp - offset) { 
            // Below target range -> Turn on heating
            if (!isHeating) {
                printf("Heating started!\n");
                isHeating = true;
                isCooling = false;
                gpio_put(HEAT_GPIO, 0); // enable heating
                gpio_put(COLD_GPIO, 1); // cooling is off
            }
        } else if (temp > target_temp + offset) { 
            // Above target range -> Turn on cooling
            if (!isCooling) {
                printf("Cooling started!\n");
                isCooling = true;
                isHeating = false;
                gpio_put(COLD_GPIO, 0); // enable cooling
                gpio_put(HEAT_GPIO, 1); // heating off
            }
        } 
        // else if (temp > target_temp - 0.5 * offset && temp < target_temp + 0.5 * offset) { 
        //     // if close to target temp, disable heating / cooling
        //     if (isHeating || isCooling) {
        //         printf("Peltier turned off (Stable temperature: %.2f°C)\n", temp);
        //         isHeating = false;
        //         isCooling = false;
        //         gpio_put(HEAT_GPIO, 1); // turn off heating
        //         gpio_put(COLD_GPIO, 1); // turn off cooling
        //     }
        // }

        // turn off heating just before reaching desired temp,
        // the produced heat will reach the target temp on its own
        if (isHeating && temp >= target_temp - 0.5 * offset) {
            printf("Predictively turning off heating (Temp: %.2f°C)\n", temp);
            isHeating = false;
            gpio_put(HEAT_GPIO, 1); // turn off heating slightly early
            target_temp = 20.0f;
        }
        if (isCooling && temp <= target_temp + 0.5 * offset) {
            printf("Predictively turning off cooling (Temp: %.2f°C)\n", temp);
            isCooling = false;
            gpio_put(COLD_GPIO, 1); // turn off cooling slightly early
            target_temp = 30.0f;
        }
    }
}

void PeltierDriver::set_target_temp(float target_temp){
    this->target_temp = target_temp;
}