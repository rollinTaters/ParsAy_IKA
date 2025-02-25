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

int main(int argc, char *argv[])
{
  std::cout << "Image Processing App" << "\n";
  
  try {
	 	 
	 if (argc > 1 && std::string(argv[1]) == "test") {
		std::cout << "Test state" << "\n";
		const std::vector<cv::String> filenames = {
		  "assets/sign_1_basic.png",
		  "assets/sign_1_not_centred.png",		  
		  "assets/sign_1_with_garbage.png",
		};
	 		
	 
	 		
		for (cv::String filename : filenames) {
		  cv::Mat image = cv::imread(filename);
	 
		  if (image.empty()) {
			 std::cerr << "Could not load image" << "\n";
			 return -19;
		  }
	 
	 	 
		  cv::Mat edges = Preprocessing::applyAll(image);
	 
		  std::vector<cv::Rect> signs = SignDetection::detectSigns(edges);
		  for (const cv::Rect &rect : signs)
			 cv::rectangle(image, rect, cv::Scalar(0, 255, 0), 2);
	 
		  cv::imshow("Screen", image);
	 
		  cv::waitKey(0);
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
