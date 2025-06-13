# run main.py to test

import math
from vector import *

IR_ANGLES = [
     3*math.pi/8,    math.pi/4,    math.pi/8,     0,
      -math.pi/8,   -math.pi/4, -3*math.pi/8, -math.pi/2,
    -5*math.pi/8, -3*math.pi/4, -7*math.pi/8,    math.pi,
     7*math.pi/8,  3*math.pi/4,  5*math.pi/8,  math.pi/2
]

class IRCalculator:
    def __init__ (self) -> None:
        self.ir_vectors = [Vector(0, 0) for i in range(16)]
        self.angle = 0
        self.magnitude = 0

    @property
    def ir_vec(self) -> Vector:
        return Vector.from_angle_magnitude(self.angle, self.magnitude)

    def calc_ir_vectors(self, ir_values:list[float]) -> list[Vector]:
        return [
            Vector.from_angle_magnitude(IR_ANGLES[i], ir_value)
            for i, ir_value in enumerate(ir_values)
        ]

    def calc_final_vector(self, ir_vectors:list[Vector]) -> Vector:
        # filtered_vectors = [ir_vector for ir_vector in ir_vectors if ir_vector.magnitude != 0]
        filtered_vectors = self.ir_vectors[0:15]
        sum_ir_vectors = Vector.sum_vectors(filtered_vectors)
        return sum_ir_vectors
    
    def update(self, ir_values:list[float]) -> None:
        self.ir_vectors = sorted(self.calc_ir_vectors(ir_values), key=lambda x: x.magnitude, reverse=True)
        final_vector = self.calc_final_vector(self.ir_vectors)
        self.angle = final_vector.heading
        self.magnitude = Vector.sum_vectors(self.ir_vectors[:8]).scale(1/8).magnitude