#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "tracking.hpp"
#include "opencv2\videoio.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"

using namespace std;
using namespace cv;

struct MouseCallbackState {
	bool is_selection_started;
	bool is_selection_finished;
	Point point_first;
	Point point_second;
};

shared_ptr<Tracker> tracker;
Mat srcImg;
Mat grayFrame;


static void onMouse(int event, int x, int y, int, void* userdata) {
	MouseCallbackState* mouseState = reinterpret_cast<MouseCallbackState*>(userdata);
	if (event == EVENT_LBUTTONDOWN) {
		mouseState->is_selection_started = true;
		mouseState->is_selection_finished = false;
		mouseState->point_first = Point(x, y);
	}
	if (event == EVENT_LBUTTONUP) {
		mouseState->is_selection_started = false;
		mouseState->is_selection_finished = true;
		mouseState->point_second = Point(x, y);
		cv::cvtColor(srcImg, grayFrame, CV_BGR2GRAY);
		tracker->Init(grayFrame, Rect(mouseState->point_first, mouseState->point_second));
	}
	if (event == EVENT_MOUSEMOVE && mouseState->is_selection_finished == false) {
		mouseState->point_second = Point(x, y);
	}

}

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

 
  String defaultVid = "C:/Users/iss2016/Documents/summer_school/itseez-ss-2016-practice/test/test_data/video/logo.mp4";
  VideoCapture video(defaultVid);
  if (!video.isOpened()) {
	  cerr << "Failed to load video." << endl;
	  return -1;
  }
  video >> srcImg;
  const string kSrcWindowName = "Tracking";
  namedWindow(kSrcWindowName);
  imshow(kSrcWindowName, srcImg);
  MouseCallbackState mouseState;
  mouseState.is_selection_started = false;
  mouseState.is_selection_finished = false;
  setMouseCallback(kSrcWindowName, onMouse, &mouseState);
  tracker = Tracker::CreateTracker("median_flow");
 
  


  for (;;) {
	  if (mouseState.is_selection_finished) {
		  video >> srcImg;
		  cv::cvtColor(srcImg, grayFrame, CV_BGR2GRAY);
		  Rect obj = tracker->Track(grayFrame);
		  rectangle(srcImg, obj, CV_RGB(100, 255, 100));
		  imshow(kSrcWindowName, srcImg);
	  }
	  if (waitKey(1) >= 0) break;
  }
  


  

  return 0;
}


