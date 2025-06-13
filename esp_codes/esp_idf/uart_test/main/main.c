#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/uart.h"

#define TX 43
#define RX 44
#define UART_PORT_NUM UART_NUM_1
#define BAUD_RATE 115200
#define UART_BUF_SIZE (2048)

void init_uart() {
    uart_config_t uart_config = {
        .baud_rate = BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    uart_param_config(UART_PORT_NUM, &uart_config);
    uart_set_pin(UART_PORT_NUM, TX, RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_PORT_NUM, UART_BUF_SIZE * 2, 0, 0, NULL, 0);
}

void init_led() {
    gpio_config_t led_config = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << 45),
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
    };
    gpio_config(&led_config);
    gpio_set_level(45, 1);
}

void app_main(void) {
    init_uart();
    init_led();
    uint8_t num = 0;

    while (1) {
        char msg[64];
        snprintf(msg, sizeof(msg), "Hello uart communication, num: %u\n", num++);
        uart_write_bytes(UART_PORT_NUM, msg, strlen(msg));
        // vTaskDelay(pdMS_TO_TICKS(500));
    }
}
