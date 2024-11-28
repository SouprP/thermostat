#ifndef BME680_H_
#define BME680_H_

#include <hardware/i2c.h>
#include <pico/stdlib.h>
#include <sensors/bme680_types.h>

/**
 *      GENERAL REGISTERS (I2C)
*/

#define BME680_CONTROL_HUMIDITY_REG     0x72
#define BME680_CONTROL_MEASURE_REG      0x74
#define BME680_RESET_REG                0xE0
#define BME680_CHIP_ID_REG              0xD0
#define BME680_CONFIG_REG               0x75

/**
 *      BME680 MODES
*/

#define BME680_SLEEP_MODE               0
#define BME680_FORCE_MODE               1
#define BME680_RESET_VAL                0xB6

/**
 *      DATA REGISTERS
*/

#define BME680_PRESSURE_MSB_REG         0x1F
#define BME680_PRESSURE_LSB_REG         0x20
#define BME680_PRESSURE_XLSB_REG        0x21

#define BME680_TEMP_MSB_REG             0x22
#define BME680_TEMP_LSB_REG             0x23
#define BME680_TEMP_XLSB_REG            0x24

#define BME680_HUMIDITY_MSB_REG         0x25
#define BME680_HUMIDITY_LSB_REG         0x26

#define BME680_GAS_RAW_MSB_REG          0x2A
#define BME680_GAS_RAW_LSB_REG          0x2B

/**
 *      STATUS REGISTERS
*/

#define BME680_MEASURE_STATUS_0_REG     0x1D

/**
 *      MACROS
*/

#define BME680_SET_REG_BITS(reg_data, bit_pos, value) \
    (((reg_data) & ~(0b111 << (bit_pos))) | (((value) & 0b111) << (bit_pos)))

#define lsb_msb_to_type(t,b,o) (t)(((t)b[o+1] << 8) | b[o])
#define lsb_to_type(t,b,o)     (t)(b[o])


class BME680{
    private:
        // I2C address of the BMP280 sensor
        uint8_t address;

        // pointer to the I2C instance
        i2c_inst_t* i2c; 

        bme680_calib_data_t params;

        void write_register(uint8_t reg, uint8_t value);
        void read_register(uint8_t reg, uint8_t* buf, uint8_t len);

        void get_calib_params();


    public:
        void init(i2c_inst_t* i2c_instance, uint8_t addr);
        void soft_reset();

        void set_temperature_oversampling(uint8_t osrs_t);
        void set_humidity_oversampling(uint8_t osrs_h);
        void set_pressure_oversampling(uint8_t osrs_p);
        // void set_gas_oversampling(uint8_t temp);
        void set_irf_filter_coefficient(uint8_t coefficient);

        void set_gas_heater(int16_t temperature, uint16_t time);


};

#endif