extern "C" {
    #include <stdio.h>
    #include <string.h>
    #include "freertos/FreeRTOS.h"
    #include "freertos/task.h"
    #include "esp_log.h"
    #include "esp_err.h"
    #include "esp_adc/adc_continuous.h"
    #include "driver/uart.h"
    // #include "driver/gpio.h"
    #include "esp_wifi.h"
    #include "nvs_flash.h"
}
// #include "pins.hpp"
#include "constants.hpp"
#include "led.hpp"
#include "sensor.hpp"
#include "calc.hpp"
// static const char *TAG = "wifi_ap";
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
void send_int_array_as_bytes(int32_t* arr, size_t len) {
    // Calculate the total size (number of bytes) to send
    size_t total_bytes = len * sizeof(int);

    // Send the array as raw bytes over UART
    uart_write_bytes(UART_PORT_NUM, (const char*)arr, total_bytes);
}

void send_float_array_as_bytes(float* arr, size_t len) {
    size_t total_bytes = len * sizeof(float);

    uart_write_bytes(UART_PORT_NUM, (const char*)arr, total_bytes);
}




extern "C" void app_main(void) {

    Led debug_led((gpio_num_t)45);
    init_uart();
    SensorRing ring;
    // ESP_ERROR_CHECK(nvs_flash_init());

    std::array<float, 16> sensor_angles = {180, -157.5, -135, -112.5, -90, -67.5, -45, -22.5, 0, 22.5, 45, 67.5, 90, 112.5, 135, 157.5};
    std::array<MathVector, 16> sensor_vectors;
    for (u_int8_t i = 0; i < 16; i++) {
        sensor_vectors[i] = MathVectorFromArgument(68, sensor_angles[i] * M_PI/180.0);
    }
    uint8_t result[EXAMPLE_READ_LEN];
    adc_digi_output_data_t *output_data = NULL;
    int32_t adc_values[2][16] = {{-1}}; // [unit][channel], safe with overprovisioned index


    while (true) {
        ring.readRaw(result, adc_values, output_data);
        
        int32_t tcrt_values[CHANNEL_NUM] = {
            adc_values[0][TCRT1],
            adc_values[0][TCRT2],
            adc_values[0][TCRT3],
            adc_values[0][TCRT4],
            adc_values[0][TCRT5],
            adc_values[1][TCRT6],
            adc_values[1][TCRT7],
            adc_values[1][TCRT8],
            adc_values[1][TCRT9],
            adc_values[1][TCRT10],
            adc_values[1][TCRT11],
            adc_values[0][TCRT12],
            adc_values[0][TCRT13],
            adc_values[0][TCRT14],
            adc_values[0][TCRT15],
            adc_values[0][TCRT16],
        };
        bool led_on = false;
        for (uint8_t i = 0; i < CHANNEL_NUM; i++) {
            if (tcrt_values[i] <= 2000) {
                led_on = true;
                break;
            }
        }
        if (led_on) {
            debug_led.set_level(1);
        }
        else {
            debug_led.set_level(0);
        }
        // while (true) {
        //     char read_value[1];
        //     uart_read_bytes(UART_PORT_NUM, &read_value, 1, pdMS_TO_TICKS(1000));
        //     if (read_value[0] == 'e') {
        //         break;
        //     }
        //     esp_restart();
        // }
        std::array<bool, CHANNEL_NUM> whites = find_white(tcrt_values, 2000);
        // std::array<bool, CHANNEL_NUM> whites = {true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};
        std::array<float, 2> result = find_line(whites, sensor_vectors);

        float send_value[2] = {result[0], result[1]};
        uart_write_bytes(UART_PORT_NUM, "e", 1);
        // send_int_array_as_bytes(tcrt_values, CHANNEL_NUM);
        send_float_array_as_bytes(send_value, 2);
        uart_wait_tx_done(UART_PORT_NUM, pdMS_TO_TICKS(100));
        // printf("%s", "should have writen the stuff");

        // for (int i = 0; i < CHANNEL_NUM; i++) {
        //     printf("%1d: %1d ", i + 1, whites[i]);
        // }
        printf("angle: %.5f, distance: %4f", result[0], result[1]);
        printf("\n");
        fflush(stdout);
    }
        // vTaskDelay(pdMS_TO_TICKS(15));
}
