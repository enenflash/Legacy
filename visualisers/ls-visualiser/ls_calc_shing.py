import math

sensor_coordinates = [
    [math.cos(angle*math.pi/180)*68, math.sin(angle*math.pi/180)*68]
    for angle in [
        180, -157.5, -135, -112.5,
        -90, -67.5, -45, -22.5,
        0, 22.5, 45, 67.5,
        90, 112.5, 135, 157.5
    ]
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