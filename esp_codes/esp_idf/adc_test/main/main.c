#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <driver/adc.h>
#include <esp_adc/adc_continuous.h>
void setup_led() {
    gpio_config_t ledConfig = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << 45),
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE
    };
    gpio_config(&ledConfig);
    gpio_set_level(45, 1);
}
void setup_adc1() {
    adc1_config_width(ADC_BITWIDTH_9);
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_12);
}
void app_main(void) {
    setup_led();
    setup_adc1();
    while (1){
        int value = adc1_get_raw(ADC_CHANNEL_0);
        printf("%d \n", value);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

}