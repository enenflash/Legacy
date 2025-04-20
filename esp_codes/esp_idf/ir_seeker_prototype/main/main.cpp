extern "C" {
    #include <stdio.h>
    #include <freertos/FreeRTOS.h>
    #include <freertos/task.h>
    #include <esp_timer.h>
}
#include "led.hpp"
#include "sensor.hpp"
extern "C" void app_main(void) {
    
    Led debug_led = Led((gpio_num_t)45);
    debug_led.set_level(1);
    SensorRing irseeker = SensorRing();
    while (1) {

        irseeker.readings.fill(0);
        
        // irseeker.update_mode_2(100);
        // printf("%lld ", esp_timer_get_time() - start_time);
        for (uint8_t i = 0; i < 16; i++) {
            int64_t start_time = esp_timer_get_time();
            while (esp_timer_get_time() - start_time < 833) {
                int64_t time = esp_timer_get_time();
                irseeker.readings[i] += !gpio_get_level((gpio_num_t)(16 - i));
                while (time == esp_timer_get_time());
            } 
        }


        std::array<uint32_t, 16> values = irseeker.readings;
        for (uint8_t i = 0; i < 16; i++) {
            printf("%4ld ", values[i]);
        }
        printf("%s", "\n");
        // vTaskDelay(pdMS_TO_TICKS(100));
    }
}