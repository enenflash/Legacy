#include <opencv2/opencv.hpp>
#include <iostream>
#include <unistd.h>

int main() {
    cv::VideoCapture cap("libcamerasrc ! video/x-raw, width=1920, height=1080, framerate=30/1, format=BGR ! videoconvert ! appsink sync=false", cv::CAP_GSTREAMER);
    
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open camera!" << std::endl;
        return -1;
    }

    for (;;) {
        cv::Mat frame;

        // Flush old frames
        for (int i = 0; i < 10; ++i) {
            cap.grab(); // Discard old frames
        }

        cap >> frame; // Capture the latest frame

        if (frame.empty()) {
            std::cerr << "Error: Could not capture an image!" << std::endl;
            return -1;
        }

        if (cv::imwrite("captured_image.jpg", frame)) {
            std::cout << "Image saved as captured_image.jpg" << std::endl;
        } else {
            std::cerr << "Error: Could not save the image!" << std::endl;
        }

        usleep(1000000); // 1-second delay
    }

    cap.release();
    return 0;
}
