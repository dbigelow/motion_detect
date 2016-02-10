#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"

using namespace cv;

static int MOVEMENT_THRESHOLD = 300;

void reduceNoise(Mat, Mat);
int findIndexOfBiggestComponent(Mat);

int main(int argc, char * argv[]) {
    VideoCapture cameraFeed(0);
    if (!cameraFeed.isOpened()) {
        std::cout << "Could not read from camera" << std::endl;
        return 1;
    }

    namedWindow("Camera", CV_WINDOW_AUTOSIZE);
    namedWindow("Mask", CV_WINDOW_AUTOSIZE);

    Mat rawCameraFrame;
    Mat backgroundSubtractMask;
    Mat componentStatistics;
    Mat componentCentroids;

    Ptr<BackgroundSubtractor> backgroundSubtractor = createBackgroundSubtractorMOG2();
    Mat erosionMask = getStructuringElement(MORPH_ELLIPSE, Size(4, 4), Point(0, 0));

    while (waitKey(33) < 0) {
        cameraFeed >> rawCameraFrame;

        backgroundSubtractor->apply(rawCameraFrame, backgroundSubtractMask);

        reduceNoise(backgroundSubtractMask, erosionMask);

        Mat maskConnected(backgroundSubtractMask.size(), CV_32S);
        int components = connectedComponentsWithStats(backgroundSubtractMask, maskConnected, componentStatistics,
                componentCentroids);

        int indexOfBiggestComponent = findIndexOfBiggestComponent(componentStatistics);

        if (componentStatistics.at<int>(indexOfBiggestComponent, CC_STAT_AREA) > MOVEMENT_THRESHOLD) {
            Point upperLeftCorner(componentStatistics.at<int>(indexOfBiggestComponent, CC_STAT_LEFT),
                    componentStatistics.at<int>(indexOfBiggestComponent, CC_STAT_TOP));

            Point lowerRightCorner(
                    componentStatistics.at<int>(indexOfBiggestComponent, CC_STAT_LEFT)
                            + componentStatistics.at<int>(indexOfBiggestComponent, CC_STAT_WIDTH),
                    componentStatistics.at<int>(indexOfBiggestComponent, CC_STAT_TOP)
                            + componentStatistics.at<int>(indexOfBiggestComponent, CC_STAT_HEIGHT));

            cv::rectangle(rawCameraFrame, upperLeftCorner, lowerRightCorner, Scalar(0, 0, 255));
        }

        imshow("Camera", rawCameraFrame);
        imshow("Mask", backgroundSubtractMask);
    }
    return 0;
}

void reduceNoise(Mat image, Mat element) {
    dilate(image, image, element);
    erode(image, image, element);
}

int findIndexOfBiggestComponent(Mat components) {
    int maxComponentSize = 0;
    int indexOfMaxComponent = 0;
    int numberOfComponents = components.size().height;

    for (int i = 1; i < numberOfComponents; i++) {
        if (components.at<int>(i, CC_STAT_AREA) > maxComponentSize) {
            indexOfMaxComponent = i;
            maxComponentSize = components.at<int>(i, CC_STAT_AREA);
        }
    }
    return indexOfMaxComponent;
}
