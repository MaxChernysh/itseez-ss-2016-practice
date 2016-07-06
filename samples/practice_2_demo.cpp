#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2\highgui.hpp"
#include "image_processing.hpp"

using namespace std;
using namespace cv;

const char* kAbout =
    "This is a practice demo with image processing";

const char* kOptions =
		"{ @image         | <none> | image to process            }"
		"{ gray           |        | convert ROI to gray scale   }"
		"{ median         |        | apply median filter for ROI }"
		"{ edges          |        | detect edges in ROI         }"
		"{ pix            |        | pixelize ROI                }"
		"{ h ? help usage |        | print help message          }";

struct MouseCallbackState {
	bool is_selection_started;
	bool is_selection_finished;
	Point point_first;
	Point point_second;
};



static void onMouse(int event, int x, int y, int, void* userdata) {
	MouseCallbackState* mouseState = reinterpret_cast<MouseCallbackState*>(userdata);
	if (event == EVENT_LBUTTONDOWN) {
		mouseState->is_selection_started = true;
		mouseState->is_selection_finished = false;
		mouseState->point_first.x = x;
		mouseState->point_first.y = y;
	}
	if (event == EVENT_LBUTTONUP) {
		mouseState->is_selection_started = false;
		mouseState->is_selection_finished = true;
		mouseState->point_second.x = x;
		mouseState->point_second.y = y;
	}
	if (event == EVENT_MOUSEMOVE) {
		mouseState->is_selection_started = true;
		mouseState->is_selection_finished = false;
		mouseState->point_first.x = x;
		mouseState->point_first.y = y;
	}

}



int main(int argc, const char** argv) {
  // Parse command line arguments.
  CommandLineParser parser(argc, argv, kOptions);
  parser.about(kAbout);

  // If help option is given, print help message and exit.
  if (parser.get<bool>("help")) {
    parser.printMessage();
    return 0;
  }

  // Read image.
  Mat src = imread(parser.get<string>(0), CV_LOAD_IMAGE_GRAYSCALE);
  if (src.empty()) {
	  cout << "Failed to open image file '" + parser.get<string>(0) + "'."
		  << endl;
	  return 0;
  }

  MouseCallbackState* mouseState = new MouseCallbackState;
  mouseState->is_selection_finished = false;
  mouseState->is_selection_started = false;
  mouseState->point_first = Point(0, 0);
  mouseState->point_second = Point(0, 0);

  // Show source image.
  const string kSrcWindowName = "Source image";
  const int kWaitKeyDelay = 1;
  namedWindow(kSrcWindowName, WINDOW_NORMAL);
  resizeWindow(kSrcWindowName, 640, 480);
  imshow(kSrcWindowName, src);

  // Set Mouse Callback
  setMouseCallback(kSrcWindowName, onMouse, mouseState);


  waitKey();
  delete mouseState;
  return 0;
}
