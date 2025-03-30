#include <iostream>

using namespace std;

#include "lib/vector/vector.hpp"
#include "lib/motor_controller/motor_controller.hpp"
#include "lib/position_system/position_system.hpp"

int main() {
    MotorController mc(1);
    mc.run_motors(100, 3.14159/6, 0);
    mc.TL.run(100);
    return 0;
}