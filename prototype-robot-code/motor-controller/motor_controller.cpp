#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

#define TL_PWM 0
#define TL_DIR 0
#define TR_PWM 0
#define TR_DIR 0
#define BL_PWM 0
#define BL_DIR 0
#define BR_PWM 0
#define BR_DIR 0

// import vector class (same vector class used for ir_calc.cpp)
class Vector {
    public:
    float i;
    float j;
    
    Vector(float new_i=0, float new_j=0) {
        this->i = new_i;
        this->j = new_j;
    }

    string display() {
        string display_i = to_string(this->i), display_j = to_string(this->j);
        if ((int)this->i == this->i) {
            display_i = to_string((int)this->i);
        }
        if ((int)this->j == this->j) {
            display_j = to_string((int)this->j);
        }
        return "<"+display_i+", "+display_j+">";
    }
    float magnitude() {
        return pow(pow(this->i, 2) + pow(this->j, 2), 0.5);
    }
    float heading() {
        return atan2(this->j, this->i);
    }

    float dot(Vector vec2) {
        return this->i*vec2.i + this->j*vec2.j;
    }
    Vector scale(float scalar) {
        return Vector(this->i*scalar, this->j*scalar);
    }

    /* heading in radians */
    static Vector from_heading(float heading, float magnitude) {
        return Vector(cos(heading)*magnitude, sin(heading)*magnitude);
    }

    static Vector sum_vectors(vector<Vector> vec_list) {
        float new_i = 0, new_j = 0;
        for (Vector vec : vec_list) {
            new_i += vec.i;
            new_j += vec.j;
        }
        return Vector(new_i, new_j);
    }
};

class Motor {
    public:
    int PWM_PIN;
    int DIR_PIN;
    Motor(int pwm_pin, int dir_pin) {
        this->PWM_PIN = pwm_pin;
        this->DIR_PIN = dir_pin;
    }
    void run(float speed_perc) {
        // hardware code here
    }
    void stop() {
        // hardware code here
    }
};

class MotorController {
    public:
    Motor TL = Motor(TL_PWM, TL_DIR);
    Motor TR = Motor(TR_PWM, TR_DIR);
    Motor BL = Motor(BL_PWM, BL_DIR);
    Motor BR = Motor(BR_PWM, BR_DIR);

    float rotation_const;

    MotorController(float rotation_constant=1) {
        this->rotation_const = rotation_constant;
    }
    private:
    array<float, 4> get_motor_speeds(float speed, float angle, float rotation) {
        Vector mv(cos(angle)*speed, sin(angle)*speed);
        float motor_ratio[4] = { 
            -mv.i - mv.j + rotation*this->rotation_const, 
            -mv.i + mv.j + rotation*this->rotation_const, 
             mv.i - mv.j + rotation*this->rotation_const, 
             mv.i + mv.j + rotation*this->rotation_const 
        };
        // Rounding Motor Ratio to Prevent Errors
        for (int i = 0; i < 4; i++) {
            motor_ratio[i] = round(motor_ratio[i] * 1000) / 1000;
        }
        int index = 0;
        for (int i = 1; i < 4; i++) {
            if (abs(motor_ratio[i]) > abs(motor_ratio[index])) {
                index = i;
            }
        }
        float highest = motor_ratio[index];
        array<float, 4> motor_speeds = { 0, 0, 0, 0 };
        if (highest == 0) { // avoid zero division
            return motor_speeds;
        }
        for (int i = 0; i < 4; i++) {
            motor_speeds[i] = motor_ratio[i]/highest*speed;
        }
        return motor_speeds;
    }
    public:
    // speed 0->100, angle and rotation in radians
    void run_motors(float speed, float angle, float rotation) {
        array<float, 4> motor_speeds = this->get_motor_speeds(speed, angle, rotation);
        // for (int i = 0; i < 4; i++) {
        //     cout << motor_speeds[i] << endl;
        // }
        this->TL.run(motor_speeds[0]);
        this->TR.run(motor_speeds[1]);
        this->BL.run(motor_speeds[2]);
        this->BR.run(motor_speeds[3]);
    }
    void run_raw(float tl_raw, float tr_raw, float bl_raw, float br_raw) {
        this->TL.run(tl_raw);
        this->TR.run(tr_raw);
        this->BL.run(bl_raw);
        this->BR.run(br_raw);
    }
    void stop_motors() {
        this->TL.stop();
        this->TR.stop();
        this->BL.stop();
        this->BR.stop();
    }
};

int main() {
    MotorController mc(1);
    // run_motors(speed, angle, rotation)
    // example run_motors(100%, PI/2, current tilt value)
    // to "rotation matrix it" input angle = angle of movement - tilt
    mc.run_motors(100, 3.14159/6, 0);
    return 0;
}