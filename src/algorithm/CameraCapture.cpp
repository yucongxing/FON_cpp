#include "CameraCapture.h"

CameraCapture::CameraCapture():cap(cv::VideoCapture(0)){

}
CameraCapture::~CameraCapture() {
    cap.release();
}

cv::Mat CameraCapture::getFrame() {
    cv::Mat frame;
    cap >> frame;
    return frame;
}
