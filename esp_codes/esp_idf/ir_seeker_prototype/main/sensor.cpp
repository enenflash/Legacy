#include "sensor.hpp"

SensorRing::SensorRing() {
    for (uint8_t i = 1; i < 17; i++) {
        gpio_config_t config = {
            .pin_bit_mask = (1ULL << i),
            .mode = GPIO_MODE_INPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE,
        };
        gpio_config(&config);
    }
    readings.fill(0);
}

void SensorRing::update_mode_1(uint32_t loops) {
    this->readings.fill(0);
    for (uint8_t i = 0; i < 16; i++) {
        for (uint8_t j = 0; j < loops; j++) {
            this->readings[i] += !gpio_get_level((gpio_num_t)(16 - i));
        }
    }
}

void SensorRing::update_mode_2(uint32_t loops) {
    this->readings.fill(0);
    for (uint8_t i = 0; i < loops; i++) {
        for (uint8_t j = 0; j < 16; j++) {
            this->readings[j] += !gpio_get_level((gpio_num_t)(16 - j)); 
        }
    }
}
