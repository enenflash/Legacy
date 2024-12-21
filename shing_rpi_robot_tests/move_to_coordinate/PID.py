import math
import time
import motor_driver
class PID:
    def __init__(self, kp, ki, kd):
        self.kp = kp
        self.ki = ki
        self.kd = kd
        self.prev_error = 0
        self.integral = 0

    def calculate(self, error, dt):
        self.integral += error * dt
        derivative = (error - self.prev_error) / dt
        self.prev_error = error
        return self.kp * error + self.ki * self.integral + self.kd * derivative
        
        
def navigate_to_target(
    current_x, current_y, 
    target_x, target_y, 
    linear_pid, max_speed, dt
):
    # Calculate the target vector and distance
    dx = target_x - current_x
    dy = target_y - current_y
    distance_to_target = math.sqrt(dx**2 + dy**2)

    # Compute linear speed using PID
    linear_speed = linear_pid.calculate(distance_to_target, dt)

    # Normalize the movement vector
    if distance_to_target > 0:
        direction_vector_x = dx / distance_to_target
        direction_vector_y = dy / distance_to_target
    else:
        direction_vector_x = 0
        direction_vector_y = 0

    # Scale the vector by linear speed and cap at max speed
    scaled_vector_x = direction_vector_x * linear_speed
    scaled_vector_y = direction_vector_y * linear_speed

    # Cap the vector's magnitude to the max speed
    vector_magnitude = math.sqrt(scaled_vector_x**2 + scaled_vector_y**2)
    if vector_magnitude > max_speed:
        scale = max_speed / vector_magnitude
        scaled_vector_x *= scale
        scaled_vector_y *= scale

    return scaled_vector_x, scaled_vector_y, max_speed