#include <peltier_driver.h>


void PeltierDriver::init(uint8_t heat_gpio, uint8_t cold_gpio, DS18B20 *ds18b20){
    // this->ds18b20 = ds18b20;
    this->ds18b20 = new DS18B20(2);
    this->HEAT_GPIO = heat_gpio;
    this->COLD_GPIO = cold_gpio;

    this->isWorking = false;

    gpio_init(HEAT_GPIO);
    gpio_init(COLD_GPIO);

    gpio_set_dir(HEAT_GPIO, GPIO_OUT);
    gpio_set_dir(COLD_GPIO, GPIO_OUT);

    gpio_put(HEAT_GPIO, 1);
    gpio_put(COLD_GPIO, 1);

    ds18b20->init();
}

void PeltierDriver::drive(){
    while(true){
        sleep_ms(1000);
        float temp = ds18b20->read_temperature();
        printf("Temp before: %.2f°C\n", temp);


        if(temp < target_temp - offset && !isWorking){
            printf("Peltier turned on!\n");
            isWorking = true;
            gpio_put(HEAT_GPIO, 0);
            continue;
        }

        if(isWorking && temp > target_temp + offset){
            isWorking = false;
            gpio_put(HEAT_GPIO, 1);
            printf("Heating: %.2f°C\n", temp);
        }

    }
}