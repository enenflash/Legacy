#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <cmath>
#include <cstdlib> // rand

using namespace std;

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

const int PI = 3.14159265;
const float IR_ANGLES[16] = {
     3*PI/8,    PI/4,    PI/8,     0,
      -PI/8,   -PI/4, -3*PI/8, -PI/2,
    -5*PI/8, -3*PI/4, -7*PI/8,    PI,
     7*PI/8,  3*PI/4,  5*PI/8,  PI/2
};

// Returns average angle and magnitude of the IR ball given all 16 IR vectors
// Note: angle in radians
class IRCalculator {
    public:
    array<Vector, 16> ir_vectors;
    float angle = 0;
    float magnitude = 0;

    IRCalculator() {
        for (int i = 0; i < 16 ; i++) {
            this->ir_vectors[i] = Vector(0, 0);
        }
    }

    void update(float ir_values[16]) {
        this->calculate_angle_magnitude(
            this->calculate_ir_vectors(ir_values)
        );
    }

    array<Vector, 16> calculate_ir_vectors(float ir_values[16]) {
        array<Vector, 16> new_ir_vectors;
        for (int i = 0; i < 16; i++) {
            if (ir_values[i] == 0) {
                new_ir_vectors[i] = Vector(0, 0);
                continue;
            }
            new_ir_vectors[i] = Vector::from_heading(IR_ANGLES[i], ir_values[i]);
        }
        return new_ir_vectors;
    }
    
    void calculate_angle_magnitude(array<Vector, 16> new_ir_vectors) {
        this->ir_vectors = new_ir_vectors;
        vector<Vector> vec_ir_vectors;
        for (int i = 0; i < 16; i++) {
            if (ir_vectors[i].magnitude() == 0) {
                continue;
            }
            vec_ir_vectors.push_back(ir_vectors[i]);
        }
        Vector sum_ir_vectors = Vector::sum_vectors(vec_ir_vectors);
        this->angle = sum_ir_vectors.heading();
        this->magnitude = 250/sum_ir_vectors.magnitude();
    }

    // returns angle in radians
    float get_angle() {
        return this->angle;
    }
    float get_magnitude() {
        return this->magnitude;
    }
    Vector get_ir_vector() {
        return Vector::from_heading(this->angle, this->magnitude);
    }
};

/* ::::::::::::::::::::::::::::::::::: test functions :::::::::::::::::::::::::::::::::::::; */

// function to test IRCalculator
array<Vector, 16> gen_test_ir_vectors() {
    float base_angle = 22.5;
    float rand_magnitude = 0;
    array<Vector, 16> test_ir_vectors;
    for (int i = 0; i < 16; i++) {
        rand_magnitude = rand() % 101;
        test_ir_vectors[i] = Vector(
            cos(base_angle*i*3.14159/180)*rand_magnitude,
            sin(base_angle*i*3.14159/180)*rand_magnitude
        );
    }
    return test_ir_vectors;
}

// ball close in front of T4
float test_set_1[16] = {
    188, 205, 214, 245,
    194, 198, 189, 151,
    132, 144, 113, 113,
    120, 115, 152, 161
};

// ball semi-far, in front of T4
float test_set_2[16] = {
    202, 209, 203, 204,
    202, 204, 154,  72,
    129, 120, 125, 119,
    139, 142,   2, 105
};

// ball far, in front of T4
float test_set_3[16] = {
    17, 0, 97, 168,
    140, 67, 96, 0,
    0, 0, 0, 0,
    0, 0, 0, 0
};

int main() { 
    cout << "\nIRCalculator test" << endl;
    IRCalculator ir_calc;

    cout << "\ntest set 1" << endl;
    ir_calc.update(test_set_1);
    cout << "angle radians = " << ir_calc.get_angle() << endl;
    cout << "angle degrees = " << ir_calc.get_angle()*180/PI << endl;
    cout << "magnitude = " << ir_calc.get_magnitude() << endl;

    cout << "\ntest set 2" << endl;
    ir_calc.update(test_set_2);
    cout << "angle radians = " << ir_calc.get_angle() << endl;
    cout << "angle degrees = " << ir_calc.get_angle()*180/PI << endl;
    cout << "magnitude = " << ir_calc.get_magnitude() << endl;

    cout << "\ntest set 3" << endl;
    ir_calc.update(test_set_3);
    cout << "angle radians = " << ir_calc.get_angle() << endl;
    cout << "angle degrees = " << ir_calc.get_angle()*180/PI << endl;
    cout << "magnitude = " << ir_calc.get_magnitude() << endl;

    return 0;
}