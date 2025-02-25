#include "VideoCapture.hpp"
#include <iostream>
#include <stdexcept>

VideoCapture::VideoCapture(int cameraIndex) {
  cap.open(cameraIndex);
  if (!cap.isOpened()) 
    throw std::runtime_error("Could not open camera");
}

VideoCapture::~VideoCapture() { cap.release(); }

bool VideoCapture::getFrame(cv::Mat &frame) {
  return cap.read(frame);
}
