#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <cmath>
#include <cstdlib> // rand

#define T1 16
#define T2 15
#define T3 14
#define T4 13 
#define T5 12
#define T6 11
#define T7 10
#define T8 9
#define T9 8
#define T10 7
#define T11 6
#define T12 5
#define T13 4
#define T14 3
#define T15 2
#define T16 1

int sensorPins[16] = {T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16};


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

void setup() {
  // Setting up computer serial
  Serial.begin(115200);
  pinMode(45, OUTPUT);
  // Displaying LED to show running
  digitalWrite(45, HIGH);

  // Setting up IR pins as input
  for (int i=0; i<16; i++) {
    pinMode(sensorPins[i], INPUT);
  }

}

void loop() {
  IRCalculator ir_calc;

  unsigned long startTime = millis();
  float readings[16] = {};
  while(millis()-startTime < 5 ) {//0.883
    for (int i=0; i<16; i++) {
      readings[i] += !digitalRead(sensorPins[i]);
    }
  }

  ir_calc.update(readings);
  Serial.print("Angle Radians: ");
  Serial.println(ir_calc.get_angle());
  Serial.print("Angle Degrees: ");
  Serial.println(ir_calc.get_angle()*180/PI);
  Serial.print("Magnitude: ");
  Serial.println(ir_calc.get_magnitude());
  Serial.println("");
}


