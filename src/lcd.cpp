#include <menu/lcd.h>

/**
 * 
 *          PROGRAM VARIABLES
 * 
*/

// uint8_t LCD_ADDR;

/**
 * 
 *          CORE FUNCTIONS
 * 
*/

void LCD::init(i2c_inst_t* i2c_instance, uint8_t addr){
    I2C_PORT = i2c_instance;
    LCD_ADDR = addr;
    this->backlight_state = true;

    // i2c_init(I2C_PORT, 100 * 1000);
    // gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    // gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    // gpio_pull_up(SDA_PIN);
    // gpio_pull_up(SCL_PIN);

    // Make the I2C pins available to picotool
    // bi_decl( bi_2pins_with_func(sda, scl, GPIO_FUNC_I2C)); For some reason this produces an error.

    command(0x03, LCD_COMMAND, 1);
    command(0x03, LCD_COMMAND, 1);
    command(0x03, LCD_COMMAND, 1);
    command(0x02, LCD_COMMAND, 1);

    command(LCD_ENTRY_MODE_SET | LCD_ENTRY_LEFT, LCD_COMMAND, 1);
    command(LCD_FUNCTION_SET | LCD_2LINE, LCD_COMMAND, 1);
    command(LCD_DISPLAY_CONTROL | LCD_DISPLAY_ON, LCD_COMMAND, 1);
    clear();
}

void LCD::i2c_write_byte(uint8_t val){
    i2c_write_blocking(I2C_PORT, LCD_ADDR, &val, 1, false);
}

void LCD::command(uint8_t val, uint8_t mode, uint8_t backlight){
    uint8_t high;
    uint8_t low;

    if(backlight)
    {
        high = mode | (val & 0xF0) | LCD_BACKLIGHT;
        low = mode | ((val << 4) & 0xF0) | LCD_BACKLIGHT;
    }
    else {
        high = mode | (val & 0xF0);
        low = mode | ((val << 4) & 0xF0);
    }

    i2c_write_byte(high);
    display_handle(high);
    i2c_write_byte(low);
    display_handle(low);
}

/**
 * 
 *           FUNCTIONALITY 
 * 
*/

void LCD::clear(){
    command(LCD_CLEAR_DISPLAY, LCD_COMMAND, backlight_state);
}

void LCD::set_cursor(uint8_t line, uint8_t position){
    uint8_t line_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
    uint8_t val = 0x80 + line_offsets[line] + position;
    command(val, LCD_COMMAND, backlight_state);
}

void LCD::print_char(char val){
    command(val, LCD_CHARACTER, backlight_state);
}

void LCD::print_str(const char *str){
    while(*str)
        print_char(*str++);
}

void LCD::print_custom(uint8_t location, uint8_t charmap[]){
    location &= 0x7; // we only have 8 locations 0-7
    command(LCD_SET_CGRAM_ADDR | (location << 3), LCD_COMMAND, 1);
    for (int i=0; i<8; i++) {
        command(charmap[i], 1, 0);
    }
}

void LCD::display_handle(uint8_t val){
    #define DELAY_US 600
    sleep_us(DELAY_US);
    i2c_write_byte(val | LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
    i2c_write_byte(val & ~LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
}

void LCD::backlight(bool enabled){
    if(enabled){
        command(0x0C, LCD_COMMAND, 1);
        backlight_state = 1;
        return;
    }

    command(0x08, LCD_COMMAND, 0);
    backlight_state = 0;
}   