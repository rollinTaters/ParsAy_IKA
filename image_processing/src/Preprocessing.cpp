#include "Preprocessing.hpp"
#include "opencv2/core/types.hpp"
#include "opencv2/imgproc.hpp"

cv::Mat Preprocessing::toGray(const cv::Mat &input) {
  cv::Mat gray;
  cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
  return gray;
}

cv::Mat Preprocessing::applyGaussianBlur(const cv::Mat &input) {
  cv::Mat blurred;
  cv::GaussianBlur(input, blurred, cv::Size(5, 5), 0);
  return blurred;
}

cv::Mat Preprocessing::detectEdges(const cv::Mat &input) {
  cv::Mat edges;
  cv::Canny(input, edges, 50, 150); // canny(data, edges(processed data), minimum gradiant val, maximum gradiant val)  
  return edges;
}

cv::Mat Preprocessing::applyAll(const cv::Mat &input) {  
		cv::Mat gray = Preprocessing::toGray(input);
		cv::Mat blurred = Preprocessing::applyGaussianBlur(gray);
		cv::Mat edges = Preprocessing::detectEdges(blurred);
		return edges;
}
