#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_err.h"
// #include "driver/adc.h"
#include "esp_adc/adc_continuous.h"

#define TAG "ADC_CONTINUOUS"
#define EXAMPLE_ADC_OUTPUT_TYPE    ADC_DIGI_OUTPUT_FORMAT_TYPE1
#define EXAMPLE_READ_LEN           64  // Bytes

// Setup channel config
static adc_channel_t channel[1] = {ADC_CHANNEL_0}; // GPIO1 on ESP32-S2
static adc_continuous_handle_t adc_handle = NULL;

void app_main(void) {
    // === 1. ADC continuous config ===
    adc_continuous_handle_cfg_t adc_config = {
        .max_store_buf_size = 1024,     // Internal DMA buffer
        .conv_frame_size = EXAMPLE_READ_LEN,
    };
    ESP_ERROR_CHECK(adc_continuous_new_handle(&adc_config, &adc_handle));

    // === 2. Pattern config ===
    adc_continuous_config_t adc_pattern_config = {
        .sample_freq_hz = 20000,
        .conv_mode = ADC_CONV_SINGLE_UNIT_1,
        .format = EXAMPLE_ADC_OUTPUT_TYPE,
    };

    adc_digi_pattern_config_t pattern[1] = {0};
    pattern[0].atten = ADC_ATTEN_DB_12;  // 0–3.3V input range
    pattern[0].channel = channel[0];
    pattern[0].unit = ADC_UNIT_1;
    pattern[0].bit_width = ADC_BITWIDTH_12;

    adc_pattern_config.pattern_num = 1;
    adc_pattern_config.adc_pattern = pattern;

    ESP_ERROR_CHECK(adc_continuous_config(adc_handle, &adc_pattern_config));
    ESP_ERROR_CHECK(adc_continuous_start(adc_handle));

    uint8_t result[EXAMPLE_READ_LEN];
    adc_digi_output_data_t *output_data;

    // === 3. Read loop ===
    while (true) {
        uint32_t ret_bytes = 0;
        esp_err_t ret = adc_continuous_read(adc_handle, result, EXAMPLE_READ_LEN, &ret_bytes, 1000);
        if (ret == ESP_OK) {
            for (int i = 0; i < ret_bytes; i += sizeof(adc_digi_output_data_t)) {
                output_data = (adc_digi_output_data_t*)&result[i];
                if (output_data->type1.channel == channel[0]) {
                    printf("ADC CH%d: %d\n", output_data->type1.channel, output_data->type1.data);
                    fflush(stdout);
                }
            }
        } else if (ret == ESP_ERR_TIMEOUT) {
            ESP_LOGW(TAG, "ADC read timeout");
        } else {
            ESP_LOGE(TAG, "ADC read failed: %s", esp_err_to_name(ret));
        }
    }

    // Optional: cleanup (never reached here in this example)
    // adc_continuous_stop(adc_handle);
    // adc_continuous_deinit(adc_handle);
}
