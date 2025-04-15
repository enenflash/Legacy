import pygame as pg
from ls_calc_isabelle import *

pg.init()

# change these values
# in order of TCRT 1 to 16
whites = [
    False, True, True, True,
    False, False, True, False,
    False, False, False, True,
    True, True, False, False
]

result = find_line(whites)
print(result)

x = math.cos(result[0]*math.pi/180)*result[1]
y = math.sin(result[0]*math.pi/180)*result[1]

running = True
screen_info = pg.display.Info()
DIM = screen_info.current_h
SCALE = 4
screen = pg.display.set_mode((DIM, DIM))
clock = pg.time.Clock()
delta_time = 1
events = pg.event.get()

while running:
    event_types = [i.type for i in events]
    if pg.QUIT in event_types:
        running = False

    event_types = [event for event in events if event.type == pg.KEYDOWN]

    delta_time = clock.tick(60)
    events = pg.event.get()

    screen.fill("BLACK")

    for i, sensor in enumerate(sensor_coordinates):
        if whites[i]:
            pg.draw.line(screen, "GREEN", (DIM/2, DIM/2), (sensor[0]*SCALE+DIM/2, sensor[1]*SCALE+DIM/2), 2)
            continue
        pg.draw.line(screen, "WHITE", (DIM/2, DIM/2), (sensor[0]*SCALE+DIM/2, sensor[1]*SCALE+DIM/2), 2)

    pg.draw.line(screen, "RED", (DIM/2, DIM/2), (x*SCALE+DIM/2, y*SCALE+DIM/2), 3)

    pg.display.flip()