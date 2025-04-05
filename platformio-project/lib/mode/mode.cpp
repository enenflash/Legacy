#include <iostream>
using namespace std;

#include "mode.hpp"

Mode::Mode() {
    this->angle = 0;
    this->speed = 100;
    this->rotation = 0;
}

float Mode::get_angle() {
    return this->angle;
}

float Mode::get_speed() {
    return this->speed;
}

float Mode::get_rotation() {
    return this->rotation;
}