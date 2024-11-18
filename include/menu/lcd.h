
#ifndef LCD_H
#define LCD_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"

// #define I2C_PORT i2c0

class LCD{
    private:
        uint8_t LCD_ADDR;
        uint8_t backlight_state;
        i2c_inst_t* I2C_PORT;

        // all commands are from here
        // https://www.handsontec.com/dataspecs/I2C_2004_LCD.pdf
        
        // commands
        static const int LCD_CLEAR_DISPLAY = 0x01;
        static const int LCD_RETUR_NHOME = 0x02;
        static const int LCD_ENTRY_MODE_SET = 0x04;
        static const int LCD_DISPLAY_CONTROL = 0x08;
        static const int LCD_CURSOR_SHIFT = 0x10;
        static const int LCD_FUNCTION_SET = 0x20;
        static const int LCD_SET_CGRAM_ADDR = 0x40;
        static const int LCD_SET_DDRAM_ADDR = 0x80;

        // flags for display entry mode
        static const int LCD_ENTRY_SHIFT_INCREMENT = 0x01;
        static const int LCD_ENTRY_LEFT = 0x02;

        // flags for display and cursor control
        static const int LCD_BLINK_ON = 0x01;
        static const int LCD_CURSOR_ON = 0x02;
        static const int LCD_DISPLAY_ON = 0x04;

        // flags for display and cursor shift
        static const int LCD_MOVE_RIGHT = 0x04;
        static const int LCD_DISPLAY_MOVE = 0x08;

        // flags for function set
        static const int LCD_5x10_DOTS = 0x04;
        static const int LCD_2LINE = 0x08;
        static const int LCD_8BIT_MODE = 0x10;

        // flag for backlight control
        static const int LCD_BACKLIGHT = 0x08;

        static const int LCD_ENABLE_BIT = 0x04;

        // modes for lcd_send_byte
        static const int LCD_CHARACTER = 1;
        static const int LCD_COMMAND = 0;

        /**
         * 
         *          CORE FUNCTIONS
         * 
        */

        /**
         * Backlight handle.
        */
        void display_handle(uint8_t val);

        /**
         * @brief Writing a single byte to the LCD I2C line.
        */
        void i2c_write_byte(uint8_t val);

        /**
         * @brief Send information to the LCD.
        */
        void command(uint8_t val, uint8_t mode, uint8_t backlight);

    public:
        /**
         * 
         *           FUNCTIONALITY 
         * 
        */

        /**
         * @brief Initialize the I2C connection and setup the LCD.
         * 
         * @param SDA_PIN SDA pin of the device.
         * @param SCL_PIN SCL pin of the device.
         * @param addr Address of the device, typically 0x27.
        */
        //void lcd_init(uint8_t SDA_PIN, uint8_t SCL_PIN, uint8_t addr);
        void init(i2c_inst_t* i2c_instance, uint8_t addr);

        /**
         * @brief Clearing the screen
        */
        void clear();

        /**
         * @brief Go to the specified location on the screen
        */
        void set_cursor(uint8_t line, uint8_t position);

        /**
         * @brief Writing a char on a LCD screen.
        */
        void print_char(char val);

        /**
         * @brief Writing a string on a LCD screen.
        */
        void print_str(const char *str);

        /**
         * @brief Writing a custom  character on a LCD screen.
        */
        void print_custom(uint8_t location, uint8_t charmap[]);

        /**
         * @brief Turning ON and OFF the backlight of the screen.
        */
        void backlight(bool enabled);

};
#endif