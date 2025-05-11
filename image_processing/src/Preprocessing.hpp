#pragma once
#include <opencv2/opencv.hpp>

// This library is all about making the data more convinient to process

class Preprocessing {
 public:
  static cv::Mat toGray(const cv::Mat &input);
  static cv::Mat applyGaussianBlur(const cv::Mat &input);
  static cv::Mat detectCircles(const cv::Mat &input);
  static cv::Mat detectEdges(const cv::Mat &input);
  static cv::Mat applyAll(const cv::Mat &input);
};
