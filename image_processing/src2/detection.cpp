#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

int main() {
    std::string gif_path = "indir (1).gif";
    cv::VideoCapture cap(gif_path);
    if (!cap.isOpened()) {
        return -1;
    }

    int frame_width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int frame_height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    cv::VideoWriter output("pembe_balon_tespiti_output.avi",
        cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
        10, cv::Size(frame_width, frame_height));

    cv::Mat frame;
    while (cap.read(frame)) {
        if (frame.empty()) break;

        cv::Mat hsv;
        cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

        cv::Scalar lower_pink(140, 50, 50);
        cv::Scalar upper_pink(170, 255, 255);
        cv::Mat mask;
        cv::inRange(hsv, lower_pink, upper_pink, mask);

        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
        cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel);
        cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);

        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(mask, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

        for (const auto& cnt : contours) {
            double area = cv::contourArea(cnt);
            if (area > 500) {
                cv::Rect rect = cv::boundingRect(cnt);
                cv::rectangle(frame, rect, cv::Scalar(0, 255, 0), 2);
                cv::putText(frame, "Pembe Balon", cv::Point(rect.x, rect.y - 10),
                    cv::FONT_HERSHEY_SIMPLEX, 0.9, cv::Scalar(0, 255, 0), 2);
            }
        }

        output.write(frame);
    }

    cap.release();
    output.release();
    return 0;
}
