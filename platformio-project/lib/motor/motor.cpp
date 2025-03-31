#include "motor.hpp"

Motor::Motor(int pwm_pin, int dir_pin) {
    this->PWM_PIN = pwm_pin;
    this->DIR_PIN = dir_pin;
}

void Motor::run(float speed) {
    // implement run
}

void Motor::stop() {
    // implement stop
}