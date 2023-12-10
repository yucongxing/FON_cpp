# pragma once

#include <opencv2/videoio.hpp>


class CameraCapture {
public:
    CameraCapture();
    cv::Mat getFrame();
    ~CameraCapture();
private:
    cv::VideoCapture cap;
};
