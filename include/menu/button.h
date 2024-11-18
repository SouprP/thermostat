#ifndef BUTTON_H_
#define BUTTON_H_

#include <pico/stdlib.h>
#include <hardware/gpio.h>

// #define DEFAULT_DEBOUNCE 200000
#define DEFAULT_DEBOUNCE 400000

class Button {
    private:
        // GPIO pin of the button
        uint gpio_pin;

        // time variables used for checking debouncing
        uint32_t debounce_time;
        absolute_time_t last_press_time;

        // last recorder state of button
        bool down_state;

        /**
         * @brief Checks if any bouncing occured.
         * 
         * @return true if it was a valid button press, false otherwise.
         */
        bool debounce();

    public:
        /**
         * @brief The PIN associated with the button.
         */
        Button(uint gpio_pin);

        /**
         * @brief Checks if the button is currently pressed.
         * 
         * @return true if is pressed, otherwise false.
         */
        bool is_pressed();

        /**
         * @brief Checks if the button was pressed since the last check.
         * 
         * @return true if was pressed, otherwise false.
         */
        bool was_down();

        /**
         * @brief Sets the debounce time.
         */
        void set_debounce_time(uint32_t debounce_time);

        /**
         * @brief Updates the button state constantly, use on another core.
         */
        void update_state();
};

#endif