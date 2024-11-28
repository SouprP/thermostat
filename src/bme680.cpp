#include <sensors/bme680.h>

void BME680::init(i2c_inst_t* i2c_instance, uint8_t addr){
    i2c = i2c_instance;
    address = addr;

    uint8_t* buff[2];
    // set humidity oversampling
    // set temp oversampling
    // set pressure oversampling
    
    // set gas wait
    // set heater

}

/**
 *      CORE FUNCTIONS 
*/

void BME680::write_register(uint8_t reg, uint8_t value){
    uint8_t buf[2] = {reg, value}; // buffer to hold register address and value
    i2c_write_blocking(i2c, address, buf, 2, false); // write to the sensor

}

void BME680::read_register(uint8_t reg, uint8_t* buf, uint8_t len){
    i2c_write_blocking(i2c, address, &reg, 1, true);  // true to keep master control of bus
    i2c_read_blocking(i2c, address, buf, len, false);  // false to release bus
}

void BME680::soft_reset(){
    write_register(BME680_RESET_REG, BME680_RESET_VAL);
}

void BME680::init(i2c_inst_t* i2c_instance, uint8_t addr){

}

/**
 *      OTHER FUNCTIONS
*/

void BME680::set_temperature_oversampling(uint8_t osrx){
    // osrs_t<7:5> 
    // read the current value of control measure register
    uint8_t ctrl_meas;
    read_register(BME680_CONTROL_MEASURE_REG, &ctrl_meas, 1);

    ctrl_meas = BME680_SET_REG_BITS(ctrl_meas, 5, osrx);

    write_register(BME680_CONTROL_MEASURE_REG, ctrl_meas);
}

void BME680::set_humidity_oversampling(uint8_t osrx){
    // osrs_h<2:0>
    // read the current value of control measure register
    uint8_t ctrl_meas;
    read_register(BME680_CONTROL_MEASURE_REG, &ctrl_meas, 1);

    ctrl_meas = BME680_SET_REG_BITS(ctrl_meas, 0, osrx);

    write_register(BME680_CONTROL_MEASURE_REG, ctrl_meas);
}

void BME680::set_pressure_oversampling(uint8_t osrx){
    // osrs_p<4:2>
    // read the current value of control measure register
    uint8_t ctrl_meas;
    read_register(BME680_CONTROL_MEASURE_REG, &ctrl_meas, 1);

    ctrl_meas = BME680_SET_REG_BITS(ctrl_meas, 2, osrx);

    write_register(BME680_CONTROL_MEASURE_REG, ctrl_meas);
}

void BME680::set_irf_filter_coefficient(uint8_t coefficient){
    // filter<4:2> in config_reg
    // read the current value of config register
    uint8_t config;
    read_register(BME680_CONFIG_REG, &config, 1);

    // clear bits 4:2
    config = ~(coefficient << 2);

    // apply the new bits without changing the other
    // something like a bitmask
    config |= ((coefficient & 0b111) << 2);

    write_register(BME680_CONFIG_REG, config);
}

void BME680::set_gas_heater(int16_t temperature, uint16_t time){
    
}

void BME680::get_calib_params(){
    
}