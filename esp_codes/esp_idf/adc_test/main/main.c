#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_adc/adc_continuous.h"

#define TAG "ADC_CONTINUOUS"
#define EXAMPLE_ADC_OUTPUT_TYPE    ADC_DIGI_OUTPUT_FORMAT_TYPE1
#define EXAMPLE_READ_LEN           32  // Smaller buffer for low-latency
#define CHANNEL_NUM                3

static adc_channel_t channels[CHANNEL_NUM] = {
    ADC_CHANNEL_0,  // GPIO1
    ADC_CHANNEL_1,  // GPIO2
    ADC_CHANNEL_2   // GPIO3
};

static adc_continuous_handle_t adc_handle = NULL;

void app_main(void) {
    adc_continuous_handle_cfg_t adc_config = {
        .max_store_buf_size = 256,
        .conv_frame_size = EXAMPLE_READ_LEN,
    };
    ESP_ERROR_CHECK(adc_continuous_new_handle(&adc_config, &adc_handle));

    adc_continuous_config_t adc_pattern_config = {
        .sample_freq_hz = 5000,  // Reduce if needed for speed
        .conv_mode = ADC_CONV_SINGLE_UNIT_1,
        .format = EXAMPLE_ADC_OUTPUT_TYPE,
    };

    adc_digi_pattern_config_t pattern[CHANNEL_NUM] = {0};
    for (int i = 0; i < CHANNEL_NUM; i++) {
        pattern[i].atten = ADC_ATTEN_DB_12;
        pattern[i].channel = channels[i];
        pattern[i].unit = ADC_UNIT_1;
        pattern[i].bit_width = ADC_BITWIDTH_12;
    }

    adc_pattern_config.pattern_num = CHANNEL_NUM;
    adc_pattern_config.adc_pattern = pattern;

    ESP_ERROR_CHECK(adc_continuous_config(adc_handle, &adc_pattern_config));
    ESP_ERROR_CHECK(adc_continuous_start(adc_handle));

    uint8_t result[EXAMPLE_READ_LEN];
    adc_digi_output_data_t *output_data;
    int adc_values[CHANNEL_NUM] = {0};

    while (true) {
        uint32_t ret_bytes = 0;
        esp_err_t ret = adc_continuous_read(adc_handle, result, EXAMPLE_READ_LEN, &ret_bytes, 100);
        if (ret == ESP_OK) {
            memset(adc_values, -1, sizeof(adc_values));  // Reset values
            for (int i = 0; i < ret_bytes; i += sizeof(adc_digi_output_data_t)) {
                output_data = (adc_digi_output_data_t*)&result[i];
                int ch = output_data->type1.channel;
                for (int j = 0; j < CHANNEL_NUM; j++) {
                    if (ch == channels[j]) {
                        adc_values[j] = output_data->type1.data;
                    }
                }
            }

            // Print on same line
            printf("ADC: ");
            for (int i = 0; i < CHANNEL_NUM; i++) {
                printf("CH%d=%4d  ", channels[i], adc_values[i]);
            }
            printf("\n");
            fflush(stdout);
        }
    }
}
