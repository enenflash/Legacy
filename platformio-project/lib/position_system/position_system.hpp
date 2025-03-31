#ifndef _POSITIONSYSTEM_HPP_
#define _POSITIONSYSTEM_HPP_

#pragma once

#include <iostream>

#include "vector.hpp"
#include "otos.hpp"
#include "ultrasonic_ps.hpp"

using namespace std;

class PositionSystem {
    PositionSystem(bool use_otos=true, bool use_ult=false);
    OTOS otos;
    UltrasonicPS ult_ps;

    bool use_otos;
    bool use_ult;

    Vector opp_goal_posv;
    Vector own_goal_posv;
    Vector posv;
    float tilt;

    Vector get_relative_to(Vector other_posv);
    Vector get_angle_to(Vector other_posv);

    Vector get_posv();
    float get_tilt();

    Vector get_opp_goal_posv();
    Vector get_opp_goal_angle();

    Vector get_own_goal_posv();
    Vector get_own_goal_angle();

    void update();
};

#endif