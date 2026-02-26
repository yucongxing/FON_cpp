#include "cameraWorker.h"

#include <QCoreApplication>
#include "FocusAnalyzer.h"

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

    std::string cascade_path =
        (QCoreApplication::applicationDirPath() + "/data/haarcascades/haarcascade_frontalface_default.xml")
            .toStdString();
    FocusAnalyzer analyzer(cascade_path);

    while (m_running) {
        cv::Mat frame = cap.getFrame();
        if (frame.empty()) continue;

        cv::Mat analyzed = frame.clone();
        if (analyzer.isLoaded()) {
            FrameAnalysis result = analyzer.analyze(frame);
            analyzer.drawResults(analyzed, result);
        }

        emit frameReady(matToQImage(frame), matToQImage(analyzed));
    }
}

QImage CameraWorker::matToQImage(const cv::Mat &mat) {
    QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
    return image.rgbSwapped();
}
