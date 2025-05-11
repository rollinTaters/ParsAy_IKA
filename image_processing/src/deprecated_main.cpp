/*#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <exception>

#include <fstream>

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

#include <comms_packets.hpp>
#include <video_feed.hpp>
#include <comms_module.hpp>

#define TEST_DATA "./assets/augmented_images"
#define ESC_KEY 27

#define LEN(x) ( sizeof(x) / sizeof(x[0]) )


#include <filesystem>
namespace fs = std::filesystem;


int main(int argc, char *argv[])
{
  //  std::setenv("GST_DEBUG", "0", 1); // Disable GStreamer debug output
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
	 
		  //		  cv::imshow("Screen", image);
	 
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

		CommsModule undef_comms( CommsModule::udp, CommsModule::random_channel1 );
		VideoFeed videofeed( 5 );

		int feed_resolution_x = videofeed.getResolutionWidth();
		int feed_resolution_y = videofeed.getResolutionHeight();

		std::vector<uchar> buffer;		
		if (!cv::imencode(".png", frame, buffer)) {
		  std::cerr << "ERROR: COULDNT DUMP THE DATA" << "\n";
		  exit(-69);
		}

		videofeed.TXFrame( buffer.data(), buffer.size() );
		std::vector<CommsPacket> tx_packets = videofeed.getTXPackets();

		for( CommsPacket p : tx_packets ) {
        if( !undef_comms.sendPacket( p, CommsModule::console_channel ) )
            std::cout<<"packet sent failed!! ";
		  std::cout << "Packet Sent: {" <<  p.data << "}" << "\n";
		}
		
		

		/*		
		// TODO Include those 
		CommsModule comms_module(CommsModule::udp, CommsModule::ip_channel);
		VideoFeed streamer(5);

		int feed_resolution_x = streamer.getResolutionWidth();  
		int feed_resolution_y = streamer.getResolutionHeight();

		std::vector<uchar> buffer;
		//		buffer.resize(1);


		//		cv::Mat edges = Preprocessing::applyAll(frame);
		//
		//		std::vector<cv::Rect> signs = SignDetection::detectSigns(edges);
		//		for (const cv::Rect &rect : signs) {
		//		  cv::rectangle(frame, rect, cv::Scalar(0, 255, 0), 2);
		//		}

		//cv::imshow("Screen", frame);
		//		cv::imshow("scren", frame);
		if (!cv::imencode(".png", frame, buffer)) {
		std::cerr << "ERROR: COULDNT DUMP THE DATA" << "\n";
		exit(-69);
		}

		cv::Mat decoded_frame = cv::imdecode(buffer, cv::IMREAD_COLOR);
		cv::imshow("decoded", decoded_frame);
		std::cout << decoded_frame.data[0] << "\n";

		streamer.TXFrame(buffer.data(), buffer.size());
		std::vector<CommsPacket> tx_packets = streamer.getTXPackets();
		for( CommsPacket p : tx_packets )
		if( !comms_module.sendPacket( p, CommsModule::ip_channel2 ) )
		std::cout<<"packet sent failed!! ";
		  
	
		

		std::ofstream out_file("output.png", std::ios::binary);
		if (!out_file) {
		std::cerr << "File open failed!" << std::endl;
		return -1;
		}

		out_file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
		out_file.close();


		


		//		for (uchar d : buffer) 
		//		  std::cout << (int)d << " ";
		//		std::cout << "\n-----------------------\n";

		streamer.TXFrame(buffer.data(), buffer.size());

		std::vector<CommsPacket> tx_packets = streamer.getTXPackets();
		int cnt = 0;
		for( CommsPacket p : tx_packets ) {
		if( !comms_module.sendPacket( p, CommsModule::ip_channel ) )
		std::cout<<"packet sent failed!! ";
		cnt++;
std::cout << "%" << (100 * cnt / tx_packets.size()) << "\n";
		  		
		  if (comms_module.packetAvailable()) {
			 CommsPacket instance;
			 comms_module.readPacket(instance);
			 streamer.receivePacket(instance);
			 if (streamer.isFrameReady()) {
				std::vector<std::uint8_t> rx_buffer(100 * 1024); // 100 KB buffer
				size_t bf_size = rx_buffer.size();
				streamer.RXFrame(rx_buffer.data(), bf_size);
				rx_buffer.resize(bf_size); // Trim excess
				streamer.RXFrame(rx_buffer.data(), bf_size);
				std::vector<uchar> decoded_buffer;
				for (size_t i = 0; i < bf_size; ++i) {
				  decoded_buffer.push_back(buffer[i]);
				}
				cv::Mat decoded_frame = cv::imdecode(decoded_buffer, 0);
				cv::imshow("ADANA", decoded_frame);

				if (cv::waitKey(30) == 27) {
				  break;
				}

			 
			 } 
		  }

		}
		  

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
*/
