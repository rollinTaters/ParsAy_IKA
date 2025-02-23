#pragma once

#include <opencv2/opencv.hpp>



class SignDetection {
public:
  static std::vector<cv::Rect> detectSigns(const cv::Mat &input);
};
