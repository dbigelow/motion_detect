# Motion Detect

This project uses OpenCV's [background subtraction module](http://docs.opencv.org/3.0.0/d1/dc5/tutorial_background_subtraction.html) as a method for detecting motion in a well-lit area monitored by your computer's webcam.

When motion is detected, if one of the areas of movement is larger than a (currently hard-coded) threshold, a recording of the webcam feed will be written to disk. The recording will be the duration of the detected motion plus an additional 1 minute afterwards. If more motion is detected before the 1 minute is up, the clock will reset and the video will be appended to the same file. Video files will be named recording_{TIMESTAMP}.avi, where timestamp is in the yyyy-mm-dd-HH-MM-SS format.

## Installation

*This project has been developed and tested on Ubuntu 14.04. It should work on earlier versions of Ubuntu, and on Windows (as long as the prerequisites can be properly installed), but installation/run directions may be different for different operating systems.*

### Prerequisites:
* [OpenCV 3](http://docs.opencv.org/3.0-rc1/df/d65/tutorial_table_of_content_introduction.html)
* A webcam
* Git
* [Cmake](https://cmake.org/overview/)

### To Build:
1. Make sure OpenCV is installed on your system.
2. Clone this repository.
  - `git clone https://github.com/dbigelow/motion_detect.git`
3. Run CMake.
  - `cd motion_detect`
  - `cmake .`
4. Run make.
  - `make`

### To Run:
Running "make" in the motion_detect directory will create an executable file in the motion_detect/bin directory. Simply run this executable through a terminal or by double-clicking in the GUI.

## Future Work
- [x] Add ability to record video whenever motion is detected.
  - [ ] Add basic GUI to allow user to decide whether or not to record, the recording length, and what size threshold to record at.
- [ ] Add ability to monitor all webcams attached to the computer.
  - [ ] Prompt user to select webcams to monitor on startup.
- [ ] Add ability to interface with IP webcams.