
#include <iostream>
#include <exception>

#include <opencv2/opencv.hpp>
#include <vector>
#include "VideoCapture.hpp"
#include "Preprocessing.hpp"
#include "SignDetection.hpp"
#include "opencv2/core/cvstd.hpp"
#include "opencv2/core/types.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

#define TEST_DATA "./assets/augmented_images"
#define ESC_KEY 27

#include <filesystem>
namespace fs = std::filesystem;


int main(int argc, char *argv[])
{
  std::cout << "Image Processing App" << "\n";  
  
  try {
	 	 
	 if (argc > 1 && std::string(argv[1]) == "test") {
		std::cout << "Test state" << "\n";
		std::vector<cv::String> input_files;

		
		for (const auto& file : fs::directory_iterator(TEST_DATA)) {
		  if (file.path().string().empty()) {
			 std::cout << "Given Path is empty" << "\n";
			 continue;
		  }
		  if (file.path().extension() == ".png")  
			 input_files.push_back(std::string(file.path()));
		}

		for (cv::String file : input_files) {
		  cv::Mat image = cv::imread(file);
	 
		  if (image.empty()) {
			 std::cerr << "Could not load image" << "\n";
			 return -19;
		  }
	 
	 	 
		  cv::Mat edges = Preprocessing::applyAll(image);

		  std::vector<cv::Vec3f> circles;
		  cv::HoughCircles(edges, circles, cv::HOUGH_GRADIENT,
								 10,      // dp: inverse ratio of resolution
								 20,     // minDist: minimum distance between circle centers
								 100,    // param1: higher threshold for Canny edge detector
								 30,     // param2: threshold for center detection
								 20,     // minRadius
								 100);   // maxRadius

		  // Draw circles
		  for (const auto& circle : circles) {
			 cv::Point center(cvRound(circle[0]), cvRound(circle[1]));
			 std::cout << "Circle Detected at: " << center.x << ":" << center.y  << std::endl;
			 int radius = cvRound(circle[2]);
			 cv::circle(image, center, radius, cv::Scalar(0, 255, 0), 2);
			 //			 cv::circle(image, center, 10, cv::Scalar(255, 0, 255), -1); // center dot
		  } 

	 
		  std::vector<cv::Rect> signs = SignDetection::detectSigns(edges);
		  for (const cv::Rect &rect : signs)
			 cv::rectangle(image, rect, cv::Scalar(0, 255, 0), 2);
	 
		  cv::imshow("Screen", image);
	 
		  int key = cv::waitKey(0);

		  if (key == ESC_KEY) return 0;
		}
		return 0;
	 
	 }
	 
    VideoCapture video;
	 cv::Mat frame;

	 while (true) {
		if (!video.getFrame(frame)) {
		  std::cerr << "Failed to capture frame!" << "\n";
		  break;
		}

		cv::Mat edges = Preprocessing::applyAll(frame);

		std::vector<cv::Rect> signs = SignDetection::detectSigns(edges);
		for (const cv::Rect &rect : signs) {
		  cv::rectangle(frame, rect, cv::Scalar(0, 255, 0), 2);
		}

		cv::imshow("Screen", frame);
		
		if (cv::waitKey(30) == 27) {
		  break;
		}
	 }
  } catch (const std::exception &err) {
	 std::cerr << "Error: " << err.what() << "\n";
	 return -19;
  }

  std::cout << "Exiting." << "\n";
 
  return 0;
}
