#ifndef _OTOS_HPP_
#define _OTOS_HPP_

#pragma once

#include <iostream>
#include "../vector/vector.hpp"
using namespace std;

class OTOS {
    public:
    void set_up();
    void set_pos(float x, float y, float rotation);
    /* return position vector of robot */
    Vector get_posv();
};

#endif