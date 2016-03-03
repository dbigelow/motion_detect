#include <iostream>
#include <ctime>
#include <stdio.h>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"

using namespace cv;

static int MOVEMENT_THRESHOLD = 9000;
static int RECORDING_LENGTH = 30;
static int FRAMES_PER_SECOND = 30;

void reduceNoise(Mat image, Mat erosionMask);
int findIndexOfBiggestComponent(Mat components);
char * getNewVideoName();
Point getUpperLeftCorner(Mat componentStatistics, int index);
Point getLowerRightCorner(Mat componentStatistics, int index);

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

    clock_t startTime = clock();
    bool recording = false;

    int width = cameraFeed.get(CAP_PROP_FRAME_WIDTH);
    int height = cameraFeed.get(CAP_PROP_FRAME_HEIGHT);
    VideoWriter videoWriter;

    while (waitKey(1000 / FRAMES_PER_SECOND) < 0) {
        cameraFeed >> rawCameraFrame;

        backgroundSubtractor->apply(rawCameraFrame, backgroundSubtractMask);

        reduceNoise(backgroundSubtractMask, erosionMask);

        Mat componentImage(backgroundSubtractMask.size(), CV_32S);
        int components = connectedComponentsWithStats(backgroundSubtractMask, componentImage, componentStatistics,
                componentCentroids);

        int indexOfBiggestComponent = findIndexOfBiggestComponent(componentStatistics);
        int areaOfBiggestComponent = componentStatistics.at<int>(indexOfBiggestComponent, CC_STAT_AREA);

        if (areaOfBiggestComponent > MOVEMENT_THRESHOLD) {
            Point upperLeftCorner = getUpperLeftCorner(componentStatistics, indexOfBiggestComponent);
            Point lowerRightCorner = getLowerRightCorner(componentStatistics, indexOfBiggestComponent);

            cv::rectangle(rawCameraFrame, upperLeftCorner, lowerRightCorner, Scalar(0, 0, 255));

            startTime = clock();
            if (!recording) {
                char * videoName = getNewVideoName();
                videoWriter.open(videoName, VideoWriter::fourcc('M', 'J', 'P', 'G'), FRAMES_PER_SECOND,
                        Size(width, height));
            }
            recording = true;
        }
        if (recording && (clock() - startTime) / CLOCKS_PER_SEC < RECORDING_LENGTH) {
            videoWriter.write(rawCameraFrame);

        } else if (recording && (clock() - startTime) / CLOCKS_PER_SEC > RECORDING_LENGTH) {
            recording = false;
            videoWriter.release();
        }

        imshow("Camera", rawCameraFrame);
        imshow("Mask", backgroundSubtractMask);
    }
    return 0;
}

void reduceNoise(Mat image, Mat erosionMask) {
    dilate(image, image, erosionMask);
    erode(image, image, erosionMask);
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

char * getNewVideoName() {
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, 80, "%Y-%m-%d-%I-%M-%S", timeinfo);
    char * videoName = new char[50];
    sprintf(videoName, "recording_%s.avi", buffer);
    return videoName;
}

Point getUpperLeftCorner(Mat componentStatistics, int index) {
    Point upperLeft(componentStatistics.at<int>(index, CC_STAT_LEFT), componentStatistics.at<int>(index, CC_STAT_TOP));
    return upperLeft;
}

Point getLowerRightCorner(Mat componentStatistics, int index) {
    Point lowerRight(
            componentStatistics.at<int>(index, CC_STAT_LEFT) + componentStatistics.at<int>(index, CC_STAT_WIDTH),
            componentStatistics.at<int>(index, CC_STAT_TOP) + componentStatistics.at<int>(index, CC_STAT_HEIGHT));
    return lowerRight;
}
