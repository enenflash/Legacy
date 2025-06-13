import pygame as pg
from ls_calc_isabelle import *

pg.init()

# change these values
# in order of TCRT 1 to 16
whites = [
    False, True, True, True,
    True, True, True, False,
    False, False, True, False,
    False, False, False, False
]

# whites = [
#     True, True, True, True,
#     True, False, False, False,
#     False, False, False, False,
#     False, False, False, True
# ]

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

points = [
    [SCALE*sensor_coordinates[i][0]+DIM/2, SCALE*sensor_coordinates[i][1]+DIM/2]
    for i in range(16)
]

line = pg.Surface((50*SCALE, 200*SCALE), pg.SRCALPHA)
pg.draw.rect(line, "WHITE", pg.Rect(0, 0, 50*SCALE, 200*SCALE))
line = pg.transform.rotate(line, -math.atan2(y, x)*180/math.pi)
line_rect = line.get_rect()
line_rect.center = (x*SCALE+DIM/2, y*SCALE+DIM/2)

while running:
    event_types = [i.type for i in events]
    if pg.QUIT in event_types:
        running = False

    event_types = [event for event in events if event.type == pg.KEYDOWN]

    delta_time = clock.tick(60)
    events = pg.event.get()

    screen.fill("BLACK")

    screen.blit(line, line_rect)

    for i, sensor in enumerate(sensor_coordinates):
        if whites[i]:
            pg.draw.circle(screen, "GREEN", points[i], SCALE+DIM/50)
            # pg.draw.line(screen, "GREEN", (DIM/2, DIM/2), (sensor[0]*SCALE+DIM/2, sensor[1]*SCALE+DIM/2), 2)
            continue
        pg.draw.circle(screen, "YELLOW", points[i], SCALE+DIM/50)
        # pg.draw.line(screen, "WHITE", (DIM/2, DIM/2), (sensor[0]*SCALE+DIM/2, sensor[1]*SCALE+DIM/2), 2)

    pg.draw.line(screen, "RED", (DIM/2, DIM/2), (x*SCALE+DIM/2, y*SCALE+DIM/2), 3)
    pg.display.flip()
