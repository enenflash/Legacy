#ifndef SENSOR_HPP
#define SENSOR_HPP


#include <array>
extern "C" {
    #include <stdio.h>
    #include <stdint.h>
    #include <driver/gpio.h>
    #include <esp_system.h>
    #include <esp_task_wdt.h>
}

class SensorRing {
    public:
    std::array<uint32_t, 16> readings;
    SensorRing();
    void update_mode_1(uint32_t loops);
    void update_mode_2(uint32_t loops);
};


#endif