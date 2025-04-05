#ifndef _BOT_DATA_HPP_
#define _BOT_DATA_HPP_

#pragma once

#include <iostream>
#include "vector.hpp"

using namespace std;

struct BotData {
    bool possession;
    float heading;
    Vector ball_vec;
    Vector pos_vec;
    Vector opp_goal_vec;
    Vector own_goal_vec;
    Vector line_vec;
};

#endif