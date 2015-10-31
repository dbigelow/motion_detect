# Motion Detect

This project uses OpenCV's [background subtraction module](http://docs.opencv.org/3.0.0/d1/dc5/tutorial_background_subtraction.html) as a method for detecting motion in a well-lit area monitored by your computer's webcam.

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
Running "make" in the motion_detect directory will create an executable file in the motion_detect/bin directory. Simply run this executable through a terminal or by double-clicking in the GUI (no paramaters are accepted by the program at this time).

## Future Work
[]Add ability to record video whenever motion is detected.
[]Add ability to monitor all webcams attached to the computer.
  []Prompt user to select webcams to monitor on startup.
[]Add ability to interface with IP webcams.