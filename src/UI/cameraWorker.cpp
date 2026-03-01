#include "cameraWorker.h"

#include <QCoreApplication>
#include <QDebug>
#include <thread>
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
    std::string cascade_dir =
        (QCoreApplication::applicationDirPath() + "/data/haarcascades").toStdString();
    FocusAnalyzer analyzer(cascade_dir);

    if (!analyzer.isLoaded()) {
        qWarning() << "[CameraWorker] Cascade files not found at:"
                   << QString::fromStdString(cascade_dir)
                   << "- detection disabled";
    }

    m_scorer.reset();

    int           frame_count    = 0;
    FrameAnalysis last_result;
    bool          prev_focused   = true;
    auto          last_score_emit = std::chrono::steady_clock::now();

    while (m_running) {
        auto frame_start = std::chrono::steady_clock::now();

        cv::Mat frame = m_cap.getFrame();
        if (frame.empty()) continue;

        cv::Mat analyzed = frame.clone();
        if (analyzer.isLoaded()) {
            if (frame_count % 3 == 0)
                last_result = analyzer.analyze(frame);
            analyzer.drawResults(analyzed, last_result);
        }
        frame_count++;

        emit frameReady(matToQImage(frame), matToQImage(analyzed));

        bool is_focused = !last_result.faces.empty() && last_result.faces[0].eyes_open;
        m_scorer.update(is_focused);

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

        // Cap frame rate at ~30 fps
        auto elapsed   = std::chrono::steady_clock::now() - frame_start;
        auto remaining = std::chrono::milliseconds(33) - elapsed;
        if (remaining > std::chrono::milliseconds(0))
            std::this_thread::sleep_for(remaining);
    }
}

QImage CameraWorker::matToQImage(const cv::Mat &mat) {
    QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
    return image.rgbSwapped();
}
