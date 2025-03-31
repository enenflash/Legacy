#include "position_system.hpp"

PositionSystem::PositionSystem(bool use_otos, bool use_ult) {
    this->use_otos = use_otos;
    this->use_ult = use_ult;
    
    this->opp_goal_posv = Vector(0, 91.5);
    this->own_goal_posv = Vector(0, -91.5);
    this->posv = Vector(0, 0);
    this->tilt = 0;
}

Vector PositionSystem::get_relative_to(Vector other_posv) {
    return Vector(other_posv.i-this->posv.i, other_posv.j-this->posv.j);
}
Vector PositionSystem::get_angle_to(Vector other_posv) {
    Vector relative_vec = this->get_relative_to(other_posv);
    return atan2(relative_vec.j, relative_vec.i);
}

Vector PositionSystem::get_posv() {
    return this->posv;
}
float PositionSystem::get_tilt() {
    return this->tilt;
}

Vector PositionSystem::get_opp_goal_posv() {
    return this->get_relative_to(this->opp_goal_posv);
}
Vector PositionSystem::get_opp_goal_angle() {
    return this->get_angle_to(this->opp_goal_posv);
}

Vector PositionSystem::get_own_goal_posv() {
    return this->get_relative_to(this->own_goal_posv);
}
Vector PositionSystem::get_own_goal_angle() {
    return this->get_angle_to(this->own_goal_posv);
}

void PositionSystem::update() {
    // get tilt from BNO or get tilt from parameter
    this->tilt = 0;

    // could potentially implement a method that uses both ultrasonics and otos
    if (use_ult) {
        this->posv = this->ult_ps.get_posv();
    }
    if (use_otos) {
        this->posv = this->otos.get_posv();
    }
}