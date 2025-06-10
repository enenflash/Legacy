/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
// #include "esp_mac.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

void app_main(void) {
    gpio_config_t gpioConfig = {
        .pin_bit_mask = (1ULL << 45),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&gpioConfig);

    uint64_t number = 0;
    while (1) {
        gpio_set_level(45, 1);
        vTaskDelay(pdMS_TO_TICKS(1000));
        gpio_set_level(45, 0);
        vTaskDelay(pdMS_TO_TICKS(1000));
        printf("%llu \n", number);
        number++;
    }
}
