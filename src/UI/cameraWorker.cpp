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

const std::vector<FocusEvent>& CameraWorker::focusEvents() const {
    return m_scorer.events();
}

void CameraWorker::run() {
    CameraCapture cap;

    std::string cascade_dir =
        (QCoreApplication::applicationDirPath() + "/data/haarcascades").toStdString();
    FocusAnalyzer analyzer(cascade_dir);

    m_scorer.reset();

    bool prev_focused       = true;
    auto last_score_emit    = std::chrono::steady_clock::now();

    while (m_running) {
        cv::Mat frame = cap.getFrame();
        if (frame.empty()) continue;

        cv::Mat analyzed = frame.clone();
        FrameAnalysis result;
        if (analyzer.isLoaded()) {
            result = analyzer.analyze(frame);
            analyzer.drawResults(analyzed, result);
        }

        emit frameReady(matToQImage(frame), matToQImage(analyzed));

        m_scorer.update(result);

        bool focused = m_scorer.isFocused();
        if (focused != prev_focused) {
            emit focusStateChanged(focused);
            prev_focused = focused;
        }

        auto now = std::chrono::steady_clock::now();
        if (now - last_score_emit >= std::chrono::seconds(1)) {
            emit focusScoreUpdated(m_scorer.realtimeScore());
            last_score_emit = now;
        }
    }
}

QImage CameraWorker::matToQImage(const cv::Mat &mat) {
    QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
    return image.rgbSwapped();
}
