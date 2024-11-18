#include <menu/button.h>

Button::Button(uint gpio_pin) : gpio_pin(gpio_pin), debounce_time(200000), last_press_time(get_absolute_time()), down_state(false) {
    gpio_init(gpio_pin);
    gpio_set_dir(gpio_pin, GPIO_IN);
}

bool Button::is_pressed() {
    if (debounce() && gpio_get(gpio_pin) == 1) {
        last_press_time = get_absolute_time();
        return true;
    }
    return false;
}

bool Button::was_down() {
    bool last_state = down_state;
    down_state = false;

    return last_state;
}

void Button::set_debounce_time(uint32_t debounce_time) {
    this->debounce_time = debounce_time;
}

bool Button::debounce() {
    absolute_time_t now = get_absolute_time();
    int64_t elapsed_time = absolute_time_diff_us(last_press_time, now);
    return elapsed_time > debounce_time;
}

void Button::update_state() {
    bool new_state = gpio_get(gpio_pin);

    if(down_state && !debounce())
        return;

    down_state = new_state;
    last_press_time = get_absolute_time();
}
