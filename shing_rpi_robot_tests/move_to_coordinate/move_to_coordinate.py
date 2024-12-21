from Emakefun_MotorHAT import Emakefun_MotorHAT, Emakefun_DCMotor, Emakefun_Servo

import time
import atexit
import qwiic_otos
from PID import PID, navigate_to_target
import math
mh = Emakefun_MotorHAT(addr=0x60)
import motor_driver
def turnOffMotors():
	mh.getMotor(1).run(Emakefun_MotorHAT.RELEASE)
	mh.getMotor(2).run(Emakefun_MotorHAT.RELEASE)
	mh.getMotor(3).run(Emakefun_MotorHAT.RELEASE)
	mh.getMotor(4).run(Emakefun_MotorHAT.RELEASE)

atexit.register(turnOffMotors)

omni = motor_driver.Omni()
otos = qwiic_otos.QwiicOTOS()
if otos.is_connected() == False:
        print("The device isn't connected to the system. Please check your connection", \
            file=sys.stderr)
otos.begin()
otos.calibrateImu()
otos.setLinearUnit(otos.kLinearUnitMeters)
# Reset the tracking algorithm - this resets the position to the origin,
# but can also be used to recover from some rare tracking errors
otos.resetTracking()


dt = 0.01
linear_pid = PID(kp=1.0, ki=0.0, kd=0.5)
target_x, target_y = 0.0, 1000.0
threshold = 10  # Distance threshold to consider target reached
max_speed = 100  # Max speed for the motors
while True:
	start_time = time.time()
	pos = otos.getPosition()
	vec = [0.0, 0.0]
	
	# omni.moveXY(20, 0.0, 1.0, pos.h, 0.0, 0.01)
	current_x, current_y = pos.x * 1000, pos.y * 1000
	vec[0], vec[1], motor_speed = navigate_to_target(current_x, current_y, target_x, target_y, linear_pid, max_speed, dt)
	# print(f"Movement vector: {vec[0], vec[1]}, Motor max speed: {motor_speed}")

	# Simulate robot movement (simplified example)
	omni.moveXY(motor_speed, vec[0], vec[1], pos.h, 0.0, 0.01)

	# Check if the robot has reached the target
	distance_to_target = math.sqrt((target_x - current_x)**2 + (target_y - current_y)**2)
	if distance_to_target < threshold:
		print("Target reached!")
		break
	
	
	print(pos.x * 1000, pos.y * 1000, distance_to_target, motor_speed, vec[0], vec[1])
	# omni.moveXY(speed, vec[0], vec[1], pos.h, 0.0, 0.05)
	end_time = time.time()
	dt = end_time - start_time