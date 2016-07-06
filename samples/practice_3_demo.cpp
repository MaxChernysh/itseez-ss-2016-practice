#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "detection.hpp"
#include "opencv2\videoio.hpp"

using namespace std;
using namespace cv;

const char* kAbout =
    "This is an empty application that can be treated as a template for your "
    "own doing-something-cool applications.";

const char* kOptions =
		"{ i image        | <none> | image to process                         }"
		"{ v video        | <none> | video to process                         }"
		"{ c camera       | <none> | camera to get video from                 }"
		"{ m model        | <none> |                                          }"
		"{ h ? help usage |        | print help message                       }";


int main(int argc, const char** argv) {
  // Parse command line arguments.
  CommandLineParser parser(argc, argv, kOptions);
  parser.about(kAbout);

  // If help option is given, print help message and exit.
  if (parser.get<bool>("help")) {
    parser.printMessage();
    return 0;
  }

  shared_ptr<Detector> detector = Detector::CreateDetector("cascade");
  //detector->Init(parser.get<string>("model"));
  std::vector<Rect> objects;
  std::vector<int>  scores;
  Mat srcImg;
  String defaultVid = "C:/Users/iss2016/Documents/summer_school/itseez-ss-2016-practice/test/test_data/video/logo.mp4";
  VideoCapture video(defaultVid);
  String defaultDec = "C:/Users/iss2016/Documents/summer_school/itseez-ss-2016-practice/test/test_data/detection/cascades/opencv_logo_cascade.xml";
  detector->Init(defaultDec);
  /*
  if (parser.has("i")) {
	  video = VideoCapture(parser.get<string>("i"));
  }
  else if (parser.has("v")) {
	  video = VideoCapture(parser.get<string>("video"));
  }
  else if (parser.has("c")) {
	  video = VideoCapture(0);
  }*/
  
  
  if (!video.isOpened()) {
	  cerr << "Failed to load video." << endl;
	  return -1;
  }
  const string kSrcWindowName = "Detection";
  namedWindow(kSrcWindowName);
  

  for (;;) {
	  video >> srcImg;
	  detector->Detect(srcImg, objects, scores);
	  /*while (objects.begin() != objects.end()) {
		  rectangle(srcImg, objects.back(), Scalar(255));
		  objects.pop_back();
	  }*/
	  for (int i = 0; i < objects.size(); ++i) {
		  rectangle(srcImg, objects[i], CV_RGB(0, 255, 0));
	  }
	  imshow(kSrcWindowName, srcImg);
	  if(waitKey(1) >= 0) break;
  }
  


  

  return 0;
}
