from picamera2 import Picamera2
import cv2
import numpy as np
# Detects the biggest yellow area and prints out the xy coordinates of the middle point.
# No video output
# Initialize Picamera2
picam2 = Picamera2()
camera_config = picam2.create_preview_configuration(main={"format": "RGB888", "size": (640, 480)})
picam2.configure(camera_config)
picam2.start()

# Define the base yellow color in BGR and tolerance
base_yellow = np.array([50, 220, 220])  # Base yellow in BGR
tolerance = 70  # Tolerance for color bounds

# Calculate the lower and upper bounds for yellow in BGR
lower_yellow = np.clip(base_yellow - tolerance, 0, 255)
upper_yellow = np.clip(base_yellow + tolerance, 0, 255)

# Minimum area for valid contours
min_area = 1000  # Adjust based on your application

while True:
    # Capture a frame
    frame = picam2.capture_array()
    frame = cv2.resize(frame, (frame.shape[1] // 2, frame.shape[0] // 2))
    # Convert the frame from RGB to BGR (since Picamera2 captures in RGB)
    frame_bgr = frame

    # Create a mask for the yellow color in BGR
    mask = cv2.inRange(frame_bgr, lower_yellow, upper_yellow)

    # Find contours in the mask
    contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    # Process only the largest contour
    if contours:
        # Find the largest contour by area
        largest_contour = max(contours, key=cv2.contourArea)

        # Ensure the contour is larger than the minimum area
        if cv2.contourArea(largest_contour) > min_area:
            # Get the bounding rectangle for the largest contour
            x, y, w, h = cv2.boundingRect(largest_contour)

            # Draw a blue rectangle around the largest contour
            #cv2.rectangle(frame_bgr, (x, y), (x + w, y + h), (255, 0, 0), 2)

            # Calculate moments to find the centroid
            moments = cv2.moments(largest_contour)
            if moments["m00"] != 0:
                cx = int(moments["m10"] / moments["m00"])
                cy = int(moments["m01"] / moments["m00"])

                # Draw a red dot at the centroid
                # cv2.circle(frame_bgr, (cx, cy), 5, (0, 0, 255), -1)

                # Label the centroid with text
                # cv2.putText(frame_bgr, f"({cx}, {cy})", (cx + 10, cy - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 0, 0), 2)
                print(cx, cy)
    # Display the frame with the drawn shapes
    #cv2.imshow("Yellow Object Detection", frame_bgr)

    # Break the loop if the user presses 'q'
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Stop the camera and close OpenCV windows
picam2.stop()
cv2.destroyAllWindows()