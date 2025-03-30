#include "ultrasonic.hpp"

Ultrasonic::Ultrasonic(int trigger_pin, int echo_pin) {
    this->TRIGGER_PIN = trigger_pin;
    this->ECHO_PIN = echo_pin;
}

int Ultrasonic::get_distance() {
    // code goes here
    return 0;
}