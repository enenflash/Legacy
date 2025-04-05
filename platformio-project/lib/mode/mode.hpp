#ifndef _MODE_HPP_
#define _MODE_HPP_

#pragma once

#include <iostream>
#include "bot_data.h"
#include "vector.hpp"
using namespace std;

class Mode {
    protected:
    float angle;
    float speed;
    float rotation;

    public:
    Mode();
    float get_angle();
    float get_speed();
    float get_rotation();
    virtual void update(BotData & self_data, BotData & other_data) = 0;
};

#endif