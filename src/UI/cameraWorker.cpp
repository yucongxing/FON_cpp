#include "cameraWorker.h"

CameraWorker::CameraWorker(QObject *parent) : QThread(parent) {}

CameraWorker::~CameraWorker() {
    stopCapture();
}

void CameraWorker::startCapture() {
    if (isRunning()) return;
    m_running = true;
    start();
}

void CameraWorker::stopCapture() {
    m_running = false;
    wait();
}

void CameraWorker::run() {
    CameraCapture cap;
    while (m_running) {
        cv::Mat frame = cap.getFrame();
        if (frame.empty()) continue;
        emit frameReady(matToQImage(frame));
    }
}

QImage CameraWorker::matToQImage(const cv::Mat &mat) {
    QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
    return image.rgbSwapped();
}
