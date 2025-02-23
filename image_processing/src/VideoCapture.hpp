#pragma once

#include "opencv2/videoio.hpp"
#include <opencv2/opencv.hpp>


// This library is all about getting the data from the camera


class VideoCapture {
public:
  VideoCapture(int cameraIndex = 0);
  ~VideoCapture();

  bool getFrame(cv::Mat &frame);

private:
  cv::VideoCapture cap;
};
