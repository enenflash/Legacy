#ifndef _POSITION_SYSTEM_HPP_
#define _POSITION_SYSTEM_HPP_

#pragma once

#include <iostream>
#include <Adafruit_BNO055.h>

#include "vector.hpp"
#include "otos.hpp"
#include "ultrasonic_ps.hpp"

using namespace std;

class PositionSystem {
    private:
    Adafruit_BNO055 bno;
    OTOS otos;
    UltrasonicPS ult_ps;

    bool use_otos;
    bool use_ult;

    bool bno_ok;

    Vector opp_goal_posv;
    Vector own_goal_posv;
    Vector posv;
    float tilt;

    public:
    PositionSystem(bool use_otos=true, bool use_ult=false);

    Vector get_relative_to(Vector other_posv);

    Vector get_posv();
    float get_tilt();

    Vector get_opp_goal_vec();
    Vector get_own_goal_vec();

    bool check_bno_ok();
    void setup();
    void update();
};

#endif