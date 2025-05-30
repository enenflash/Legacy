#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <iostream>
#include <chrono>
#include <thread>

// float clip(float n, float lower, float upper) {
//     return std::max(lower, std::min(n, upper));
// }

void find_colour(cv::Mat frame, int base_colour[3], float tollerance, float min_area, int *point_x, int *point_y, char colour_name[10] = "", bool draw = true) {
    // Directly clamp values (remove std::min/std::max overhead)
    cv::Scalar lower_colour = cv::Scalar(std::max(base_colour[0] - tollerance, 0.f), std::max(base_colour[1] - tollerance, 0.f), std::max(base_colour[2] - tollerance, 0.f));
    cv::Scalar upper_colour = cv::Scalar(std::min(base_colour[0] + tollerance, 255.f), std::min(base_colour[1] + tollerance, 255.f), std::min(base_colour[2] + tollerance, 255.f));

    cv::Mat mask;
    std::vector<std::vector<cv::Point>> contours;
    cv::inRange(frame, lower_colour, upper_colour, mask);  // Mask the image
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);  // Find contours

    double max_area = 0;
    int max_area_index = -1;
    for (int i = 0; i < contours.size(); i++) {
        double area = cv::contourArea(contours[i]);
        if (area > max_area && area > min_area) {  // Only consider large enough contours
            max_area = area;
            max_area_index = i;
        }
    }

    if (max_area_index != -1) {
        cv::Rect bounding_rect = cv::boundingRect(contours[max_area_index]);
        if (draw) {
            cv::drawContours(frame, contours, max_area_index, cv::Scalar(255, 0, 0), 2);
            cv::rectangle(frame, bounding_rect, cv::Scalar(0, 255, 0), 2);
            cv::putText(frame, colour_name, cv::Point(bounding_rect.x, bounding_rect.y), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2);
        }
        *point_x = bounding_rect.x + bounding_rect.width / 2;
        *point_y = bounding_rect.y + bounding_rect.height / 2;
    } else {
        *point_x = -1;
        *point_y = -1;
    }
}

void find_blue(cv::Mat frame, int* blue_point_x, int* blue_point_y) {
    int base_blue[3] = {250, 20, 20};
    find_colour(frame, base_blue, 30, 100, blue_point_x, blue_point_y, "blue", false);
}

void find_yellow(cv::Mat frame, int* yellow_point_x, int* yellow_point_y) {
    int base_yellow[3] = {50, 232, 235};
    find_colour(frame, base_yellow, 50, 100, yellow_point_x, yellow_point_y, "yellow", false);
}

int main() {
    // Open the default camera (0)
    cv::VideoCapture cam("libcamerasrc ! video/x-raw, width=1920, height=1080, framerate=30/1, format=BGR ! videoconvert ! appsink sync=false", cv::CAP_GSTREAMER);
    
    // Check if the camera opened successfully
    if (!cam.isOpened()) {
        std::cerr << "Error: Could not open the camera!" << std::endl;
        return -1;
    }

    cv::Mat frame;
    int blue_point[2];
    int yellow_point[2];
    // Capture frames in a loop
    while (true) {
        auto start = std::chrono::high_resolution_clock::now(); // Start time

        cam >> frame;  // Capture a new frame
        cv::resize(frame, frame, cv::Size(frame.cols / 4, frame.rows / 4));
        // Check if the frame is empty (camera error)
        if (frame.empty()) {
            std::cerr << "Error: Empty frame!" << std::endl;
            break;
        }

        // Multithreading: Process blue and yellow colors in parallel
        std::thread blue_thread(find_blue, frame, &blue_point[0], &blue_point[1]);
        std::thread yellow_thread(find_yellow, frame, &yellow_point[0], &yellow_point[1]);

        // Wait for both threads to finish
        blue_thread.join();
        yellow_thread.join();

        auto end = std::chrono::high_resolution_clock::now(); // End time
        std::chrono::duration<double, std::milli> ms = end - start;  // Milliseconds
        std::cout << "Loop took: " << ms.count() << " ms" << std::endl;
        cv::Point center(frame.cols / 2, frame.rows / 2);
        std::cout << "Yellow: (" << yellow_point[0] * 4 << ", " << yellow_point[1] * 4 << "), Blue: (" << blue_point[0] * 4 << ", " << blue_point[1] * 4 << "), " << "center colours" << frame.at<cv::Vec3b>(center.y, center.x) << std::endl;
        cv::circle(frame, center, 5, cv::Scalar(0, 0, 255), 2);
        cv::imwrite("image.jpg", frame);
        // Optional: Display the captured frame (if needed)
        // cv::imshow("Camera Feed", frame);
        // if (cv::waitKey(1) == 'q') {
        //     break;
        // }
    }

    // Release the camera
    cam.release();
    return 0;
}
