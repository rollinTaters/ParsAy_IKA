#include "SignDetection.hpp"
#include "opencv2/core/types.hpp"
#include "opencv2/imgproc.hpp"
#include <vector>

std::vector<cv::Rect> SignDetection::detectSigns(const cv::Mat &input) {
  std::vector<std::vector<cv::Point>> contours;
  cv::findContours(input, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
  
  std::vector<cv::Rect> signRects;

  for (const std::vector<cv::Point> &contour : contours) {
	 cv::Rect rect = cv::boundingRect(contour);
	 if (rect.width > 50 && rect.height > 50) {
		signRects.push_back(rect);
	 }
  }
  
  return signRects;
}
