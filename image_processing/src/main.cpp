#include "opencv2/highgui.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>

#include "VideoCapture.hpp"
#include "opencv2/imgcodecs.hpp"

#include <comms_packets.hpp>
#include <video_feed.hpp>
#include <comms_module.hpp>
#include <thread>

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>


int main(int argc, char *argv[])
{
  (void) argc;
  (void) argv;

  std::cout << "Image Processing App" << "\n";

  VideoCapture video;
  cv::Mat frame;

  while (true) {
	 if (!video.getFrame(frame)) {
		std::cerr << "Failed to capture frame!" << "\n";
		break;
	 }
		
	 //		cv::imshow("deneme", frame);
	 CommsModule undef_comms( CommsModule::udp, CommsModule::img_proc_sender );
	 VideoFeed videofeed( 5 );

	 cv::resize(frame, frame, cv::Size(), 0.3, 0.3, cv::INTER_AREA);

	 // AS LOWER AS LOSSY AS FK	
	 std::vector<int> compression_params = { cv::IMWRITE_JPEG_QUALITY, 50 }; 
	

	 std::vector<uchar> buffer;
	 if (!cv::imencode(".jpg", frame, buffer, compression_params)) {
		std::cerr << "ERROR: COULDNT DUMP THE DATA" << "\n";
		exit(-69);
	 }
	 
	 cv::Mat decodedFrame = cv::imdecode(buffer, 1);
	 std::cout << buffer.size() << "\n";

	 /* // DEBUG
	 cv::imshow("DECODED FRAME", decodedFrame);

	 if (cv::waitKey(30) == 27)
		break;
	 */


	 std::cout << "BUFFER SIZE: " << buffer.size() << "\n";

	 videofeed.TXFrame( buffer.data(), buffer.size() );
	 std::vector<CommsPacket> tx_packets = videofeed.getTXPackets();

	 for( CommsPacket p : tx_packets ) {
		if( !undef_comms.sendPacket( p, CommsModule::console_channel))
		  std::cout<<"packet sent failed!! ";
		else {
		  std::cout << "Packet Sent: {" ;
		  for (int by : p.data) 
			 std::cout << by << " ";		  
		  std::cout << "}\n" ;
		}
	 }
	 std::this_thread::sleep_for(std::chrono::milliseconds(100));


  }


  return 0;
}
