import cv2
import time
from picamera2 import Picamera2
picam2 = Picamera2()
picam2.preview_configuration.main.size=(800, 800)
picam2.preview_configuration.main.format = "RGB888"
picam2.start()
# opencv does not support the raspberry pi camera normally
# so we have to use this other picamera2 library to read it
# and then pass it onto opencv
while True:
	im = picam2.capture_array()
	im = cv2.resize(im, (0,0), fx = 0.5, fy = 0.5)
	cv2.imshow("preview", im)
	if cv2.waitKey(1)==ord('q'):
		break
picam2.stop()
cv2.destroyAllWindows()