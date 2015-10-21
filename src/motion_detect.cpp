#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"

using namespace cv;

int main(int argc, char * argv[]) {
	VideoCapture cap(0);
	if(!cap.isOpened()) {
		std::cout << "Could not read from camera" << std::endl;
		return 1;
	}

	namedWindow("Camera", 1);
	namedWindow("Mask", 1);
	namedWindow("Background", 1);

	Mat frame;
	Mat fgMaskMOG2;
	Mat background;

	Ptr<BackgroundSubtractor> pMOG2;
	pMOG2 = createBackgroundSubtractorMOG2();
  	Mat element = getStructuringElement( MORPH_ELLIPSE,
                       Size(2, 2),
                       Point(0, 0));
	while(waitKey(20) < 0) {
		cap >> frame;

		pMOG2->apply(frame, fgMaskMOG2);
		pMOG2->getBackgroundImage(background);
		
		erode(fgMaskMOG2, fgMaskMOG2, element);
		erode(fgMaskMOG2, fgMaskMOG2, element);
		dilate(fgMaskMOG2, fgMaskMOG2, element);
		dilate(fgMaskMOG2, fgMaskMOG2, element);


		imshow("Camera", frame);
		imshow("Mask", fgMaskMOG2);
		imshow("Background", background);
	}
	return 0;
}