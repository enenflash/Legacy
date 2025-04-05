# shing code translated on 5th april
# run main.py to test

import math

sensor_coordinates = [
    [math.cos(math.pi), math.sin(math.pi)],
    [math.cos(-7 * math.pi / 8.0), math.sin(-7 * math.pi / 8.0)],
    [math.cos(-3 * math.pi / 4.0), math.sin(-3 * math.pi / 4.0)],
    [math.cos(-5 * math.pi / 8.0), math.sin(-5 * math.pi / 8.0)],
    [math.cos(-1 * math.pi / 2.0), math.sin(-1 * math.pi / 2.0)],
    [math.cos(-3 * math.pi / 8.0), math.sin(-3 * math.pi / 8.0)],
    [math.cos(-1 * math.pi / 4.0), math.sin(-1 * math.pi / 4.0)],
    [math.cos(-1 * math.pi / 8.0), math.sin(-1 * math.pi / 8.0)],
    [math.cos(0.0), math.sin(0.0)],
    [math.cos(1 * math.pi / 8.0), math.sin(1 * math.pi / 8.0)],
    [math.cos(1 * math.pi / 4.0), math.sin(1 * math.pi / 4.0)],
    [math.cos(3 * math.pi / 8.0), math.sin(3 * math.pi / 8.0)],
    [math.cos(1 * math.pi / 2.0), math.sin(1 * math.pi / 2.0)],
    [math.cos(5 * math.pi / 8.0), math.sin(5 * math.pi / 8.0)],
    [math.cos(3 * math.pi / 4.0), math.sin(3 * math.pi / 4.0)],
    [math.cos(7 * math.pi / 8.0), math.sin(7 * math.pi / 8.0)],
]

def find_line(whites: list[bool]) -> tuple[float, float]:
    max_dist = 0
    sensor_1 = 0
    sensor_2 = 0
    mid_point = (0, 0)
    for i in range(16):
        for j in range(16):
            if not (whites[i] and whites[j]):
                continue

            sensor_dist_x = sensor_coordinates[i][0] * 100.0 - sensor_coordinates[j][0] * 100.0
            sensor_dist_y = sensor_coordinates[i][1] * 100.0 - sensor_coordinates[j][1] * 100.0
            current_dist = math.sqrt(sensor_dist_x**2.0 + sensor_dist_y** 2.0)
            if current_dist > max_dist:
                max_dist = current_dist;
                sensor_1 = i + 1;
                sensor_2 = j + 1;
                mid_point = (sensor_coordinates[i][0] + sensor_coordinates[j][0]) / 2.0, (sensor_coordinates[i][1] + sensor_coordinates[j][1]) / 2.0

    angle = math.atan2(mid_point[1], mid_point[0]) * 180 / math.pi
    if mid_point[1] < 0.0:
      angle += 180.0;
    distance = math.sqrt(mid_point[0]**2.0 + mid_point[1]**2.0)
    return angle, distance