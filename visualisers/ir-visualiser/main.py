# run this file
# change or add test sets (such as test_set_1) and follow same format
# raw analog ir values in order of TSSP 1 to 16
# white lines are the raw sensor analog readings
# the red line shows the calculated angle and "strength"

import pygame as pg
from ir_calc import *
import serial
port = 'COM10'
baud_rate = 115200  # Adjust this if your device uses a different rate
timeout = 1  # seconds
pg.init()

test_set_1 = [
    188, 205, 214, 245, 194, 198, 189, 151,
    132, 144, 113, 113, 120, 115, 152, 161
]

test_set_2 = [
    202, 209, 203, 204, 202, 204, 154,  72,
    129, 120, 125, 119, 139, 142,   2, 105
]

test_set_3 = [
    17, 0, 97, 168, 140, 67, 96, 0,
    0, 0, 0, 0, 0, 0, 0, 0
]

test_set_4 = [
    0, 127, 198, 181, 160, 108, 0, 87, 0, 0, 0, 0, 0, 123, 0, 0,
]
test_set_5 = [
    100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
]

ir_calc = IRCalculator()

# print("\ntest set 1")
ir_calc.update(test_set_4)
print("angle", ir_calc.angle)
print("angle degrees", ir_calc.angle*180/math.pi)
print("magnitude", ir_calc.magnitude)

# print("\ntest set 2")
# ir_calc.update(test_set_2)
# print("angle", ir_calc.angle)
# print("angle degrees", ir_calc.angle*180/math.pi)
# print("magnitude", ir_calc.magnitude)

# print("\ntest set 3")
# ir_calc.update(test_set_3)
# print("angle", ir_calc.angle)
# print("angle degrees", ir_calc.angle*180/math.pi)
# print("magnitude", ir_calc.magnitude)

running = True
screen_info = pg.display.Info()
DIM = screen_info.current_h
SCALE = 1
screen = pg.display.set_mode((DIM, DIM))
clock = pg.time.Clock()
delta_time = 1
events = pg.event.get()

while running:
    event_types = [i.type for i in events]
    if pg.QUIT in event_types:
        running = False
    try:
        with serial.Serial(port, baud_rate, timeout=timeout) as ser:
            print(f"Listening on {port}...")
            line = ser.readline().decode('utf-8').strip()
            if not line:
                continue
            try:
                # Split line into list of strings, remove spaces, convert to integers
                values = [int(x.strip()) for x in line.split(',') if x.strip()]
                if len(values) == 16:
                    print("Received:", values)
                    ir_calc.update(values)
                else:
                    print(f"Ignored: wrong number of values ({len(values)}):", values)
            except ValueError:
                print("Ignored: non-integer value received:", line)
    except serial.SerialException as e:
        print(f"Could not open serial port {port}: {e}")

    
    print("angle", ir_calc.angle)
    print("angle degrees", ir_calc.angle*180/math.pi)
    print("magnitude", ir_calc.magnitude)
    event_types = [event for event in events if event.type == pg.KEYDOWN]

    delta_time = clock.tick(60)
    events = pg.event.get()

    screen.fill("BLACK")
    print("drawing ir vectors")
    lis = ir_calc.ir_vectors[0:15]
    for ir_vector in lis:
        pg.draw.line(screen, "WHITE", (DIM/2, DIM/2), (ir_vector.i*SCALE+DIM/2, -ir_vector.j*SCALE+DIM/2), 2)

    pg.draw.line(screen, "RED", (DIM/2, DIM/2), (ir_calc.ir_vec.i*SCALE+DIM/2, -ir_calc.ir_vec.j*SCALE+DIM/2), 3)

    pg.display.flip()