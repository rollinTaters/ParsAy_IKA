/*



I DID TESTING ON GUVEN'S CODE (JUST ON MY BRANCH)

SO THIS FILE IS JUST A CHEATSHEET FOR NETWORK SETUP



*/


#include "opencv2/highgui.hpp"
#include <comms_packets.hpp>
#include <iostream>
#include <video_feed.hpp>
#include <comms_module.hpp>

#include <opencv2/opencv.hpp>

int main(int argc, char *argv[])
{
  (void) argc;
  (void) argv;
  
  CommsModule comms_module(CommsModule::udp, CommsModule::img_proc_receiver);
  VideoFeed streamer( 5 );    
  CommsPacket packet; 
  size_t video_frame_size = 3159;
  std::uint8_t *video_frame_raw = new std::uint8_t[100024]{0};

  
  while (true) {
	 if (comms_module.packetAvailable()) {
		if (comms_module.readPacket(packet)) {
		  streamer.receivePacket( packet );
		  if( streamer.isFrameReady() ) {

			 streamer.RXFrame( video_frame_raw, video_frame_size );

				
			 std::cout << "HEEEY" << "\n";
			 std::cout << "FRAME SIZE: " << video_frame_size << "\n";

			 cv::Mat raw_data_mat(1, video_frame_size, CV_8UC1, video_frame_raw);
			 cv::Mat frame = cv::imdecode(raw_data_mat, cv::IMREAD_COLOR);

			 cv::imshow("screen", frame);

			 if (cv::waitKey(30) == 27) {
				break;

			 }
		  }
		}
	 }
	
  }
  
  return 0;
}
