#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <cmath>

// rand
#include <cstdlib>

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

// vector<Vector>(16, 0)

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

    void set_new_ir_vectors(array<Vector, 16> new_ir_vectors) {
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
        this->magnitude = sum_ir_vectors.magnitude();
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

class IR {
    // hardware code goes in this class
    // public array<Vector, 16> get_ir_vectors() {
    //     IRCalculator takes the return of this function
    //     (everything else is private)
    // }
};

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

int main() { 
    cout << "\nIRCalculator test" << endl;
    IRCalculator ir_calc;
    array<Vector, 16> test_vectors = gen_test_ir_vectors();
    for (Vector vec : test_vectors) {
        cout << "test_vector " + vec.display() << endl;
    }
    ir_calc.set_new_ir_vectors(gen_test_ir_vectors());
    cout << "angle = " << ir_calc.get_angle() << endl;
    cout << "magnitude = " << ir_calc.get_magnitude() << endl;

    return 0;
}