#ifndef _ULTRASONICPS_HPP_
#define _ULTRASONICPS_HPP_

#pragma once

#include <iostream>
#include <cmath>
#include <string>
#include <array>

#include "pins.hpp"
#include "constants.hpp"
#include "vector.hpp"
#include "ultrasonic.hpp"

using namespace std;

class UltrasonicPS {
    private:
    Ultrasonic ult_left = Ultrasonic(UL_TRIG, UL_ECHO);
    Ultrasonic ult_right = Ultrasonic(UR_TRIG, UR_ECHO);
    Ultrasonic ult_back = Ultrasonic(UB_TRIG, UB_ECHO);

    float ult_distances[3] = { 0, 0, 0 };
    float previous_distances[3] = { 0, 0, 0 };

    bool equals_total_width(float left_dist, float right_dist);
    
    public:
    bool reliable = true;
    bool use_left = true;

    float x, y;
    bool choose_ult();
    void update(float angle_offset);
    Vector get_posv();
};

#endif