extern "C" {
    #include <stdio.h>
    #include <freertos/FreeRTOS.h>
    #include <freertos/task.h>
    #include <driver/gpio.h>
}
class Led {
    public:
        gpio_num_t led_pin;
        Led(gpio_num_t gpio_num) {
            led_pin = gpio_num;
            gpio_config_t config = {
                .pin_bit_mask = (1ULL << led_pin),
                .mode = GPIO_MODE_INPUT_OUTPUT,
                .pull_up_en = GPIO_PULLUP_DISABLE,
                .pull_down_en = GPIO_PULLDOWN_DISABLE,
                .intr_type = GPIO_INTR_DISABLE,
            };
            gpio_config(&config);
        }
        void set_level(bool level) {
            gpio_set_level(this->led_pin, level);
        }
        void toggle() {
            set_level(!gpio_get_level(this->led_pin));
        }
};

extern "C" void app_main(void) {
    Led debug_led((gpio_num_t)45);
    debug_led.set_level(1);
    while (1) {
        printf("%s", "hello world in c++ \n");
        vTaskDelay(pdMS_TO_TICKS(1000));
        debug_led.toggle();
    }
}