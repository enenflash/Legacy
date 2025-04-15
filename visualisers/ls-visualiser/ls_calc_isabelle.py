import math

class Vector:
    def __init__ (self, i:float, j:float) -> None:
        self.i = i
        self.j = j

    @property
    def magnitude(self) -> float:
        return (self.i**2 + self.j**2)**0.5
    
    @property
    def argument(self) -> float:
        return math.atan2(self.j, self.i)
    
    def dot(self, vec:"Vector") -> float:
        return self.i*vec.i + self.j*vec.j
    
    def scalar_proj_on(self, vec:"Vector") -> float:
        if vec.dot(vec) == 0:
            return 0
        return self.dot(vec)/vec.dot(vec)

    @staticmethod
    def from_argument(magnitude:float, argument:float) -> "Vector":
        """
        argument in radians
        """
        return Vector(math.cos(argument)*magnitude, math.sin(argument)*magnitude)

sensor_vectors = [
    Vector.from_argument(68, angle*math.pi/180)
    for angle in [
        180, -157.5, -135, -112.5,
        -90, -67.5, -45, -22.5,
        0, 22.5, 45, 67.5,
        90, 112.5, 135, 157.5
    ]
]

sensor_coordinates = [
    (vec.i, vec.j)
    for vec in sensor_vectors
]

def find_line(whites: list[bool]) -> tuple[float, float]:
    sum_vector = Vector(0, 0)
    distances = []
    for i in range(16):
        if not whites[i]:
            continue
        sum_vector.i += sensor_vectors[i].i
        sum_vector.j += sensor_vectors[i].j
    
    if sum_vector.i == 0 and sum_vector.j == 0:
        return 0, 0
    
    angle = sum_vector.argument
    line_vec = Vector.from_argument(1, angle)

    distances = [sensor_vectors[i].scalar_proj_on(line_vec) for i in range(16) if whites[i]]
    distance = sum(distances)/len(distances)
    
    return angle*180/math.pi, distance