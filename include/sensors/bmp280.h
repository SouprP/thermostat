#ifndef BMP280_H_
#define BMP280_H_

#include <hardware/i2c.h>
#include <pico/stdlib.h>

// structure to hold calibration parameters read from the sensor
struct bmp280_calib_param {
    // temperature params
    uint16_t dig_t1;
    int16_t dig_t2;
    int16_t dig_t3;

    // pressure params
    uint16_t dig_p1;
    int16_t dig_p2;
    int16_t dig_p3;
    int16_t dig_p4;
    int16_t dig_p5;
    int16_t dig_p6;
    int16_t dig_p7;
    int16_t dig_p8;
    int16_t dig_p9;
};

class BMP280 {
private:
    // I2C address of the BMP280 sensor
    uint8_t address;

    // pointer to the I2C instance
    i2c_inst_t* i2c; 

    // structure to hold calibration parameters read from the sensor
    struct bmp280_calib_param calib_params;

    /**
     * @brief Writes a value to a register.
     * 
     * @param reg register which we want to write to.
     * @param value value which will be written.
     */
    void write_register(uint8_t reg, uint8_t value);

    /**
     * @brief Read value from a register.
     * 
     * @param reg register from which we want to read value from
     * @param buf buffer to where the data will be written.
     * @param len length of data to be read.
     */
    void read_registers(uint8_t reg, uint8_t* buf, uint8_t len);

    /**
     * @brief Callibration parameters from BMP280 sensor.
     */
    void get_calib_params();

    /**
     * @brief Converts raw temperature to actual temperature in degrees Celcius.
     * 
     * @param raw_temp raw temperature read from read_raw().
     * @return temperature value.
     */
    int32_t convert_temp(int32_t raw_temp);

    /**
     * @brief Converts raw pressure to actual pressure in hPa.
     * 
     * @param raw_pressure raw pressure read from read_raw().
     * @return pressure value.
     */
    int32_t convert_pressure(int32_t raw_pressure, int32_t raw_temp);

public:
    /**
     * @brief Initialization of the BMP280 sensor with a given I2C instance and address.
     * 
     * @param i2c_instance I2C instance.
     * @param addr address of our BMP280.
     */
    void init(i2c_inst_t* i2c_instance, uint8_t addr = 0x76);

    /**
     * @brief Read raw data from the sensor.
     * 
     * @param raw_temp raw temperature data.
     * @param raw_pressure raw pressure data.
     */
    void read_raw(int32_t* raw_temp, int32_t* raw_pressure);

    /**
     * @brief Gets the temperature value in Celcius.
     * 
     * @return temperature in Celcius.
     */
    float get_temperature();

    /**
     * @brief Gets the temperature value in hPa.
     * 
     * @return pressure in hPa.
     */
    float get_pressure();
};

#endif // BMP280_H_
