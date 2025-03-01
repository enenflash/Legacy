import cv2
import numpy as np
from picamera2 import Picamera2

# draw green contours around the detected shape.
# draw a cyan rectangle around that and a red dot in the middle.
# variable constants
base_yellow = np.array([0, 180, 205])

tolerance = 60

lower_yellow = np.clip(base_yellow - tolerance, 0, 255)
upper_yellow = np.clip(base_yellow + tolerance, 0, 255)

min_area = 500

# setting up the camera
picam2 = Picamera2()
picam2.preview_configuration.main.size=(800, 800)
picam2.preview_configuration.main.format = "RGB888"
picam2.start()
# opencv does not support the raspberry pi camera normally
# so we have to use this other picamera2 library to read it
# and then pass it onto opencv


while True:
	im = picam2.capture_array()
	frame = cv2.resize(im, (0,0), fx = 0.5, fy = 0.5)
	mask = cv2.inRange(frame, lower_yellow, upper_yellow)
	
	contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
	print(len(contours))
	for contour in contours:
		# Calculate the area of the contour

		area = cv2.contourArea(contour)
		# Only proceed if the area is greater than the minimum threshold
		if area > min_area:
			# Calculate the moments to find the centroid
			x,y,w,h = cv2.boundingRect(contour)
			cv2.rectangle(frame,(x,y),(x+w,y+h),(255,255,0),2)
			moments = cv2.moments(contour)
			if moments["m00"] != 0:
				cx = int(moments["m10"] / moments["m00"])
				cy = int(moments["m01"] / moments["m00"])

				# Draw the contour (optional, for visualization)
				cv2.drawContours(frame, [contour], -1, (0, 255, 0), 2)  # Green contour
				# Mark the centroid with a red circle
				cv2.circle(frame, (cx, cy), 7, (0, 0, 255), -1)  # Red circle
				# Label the centroid
				cv2.putText(frame, f'({cx}, {cy})', (cx + 10, cy - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 0, 0), 2)
	cv2.imshow("yellow detection", frame)
	if cv2.waitKey(1)==ord('q'):
		break
picam2.stop()
cv2.destroyAllWindows()
