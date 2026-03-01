#pragma once

#include <QImage>
#include <QThread>
#include <atomic>

#include "CameraCapture.h"
#include "FocusScorer.h"

class CameraWorker : public QThread {
    Q_OBJECT
public:
    explicit CameraWorker(QObject *parent = nullptr);
    ~CameraWorker() override;

signals:
    void frameReady(const QImage &original, const QImage &analyzed);
    void focusStateChanged(bool is_focused);
    void focusScoreUpdated(int score);

public slots:
    void startCapture();
    void stopCapture();

protected:
    void run() override;

private:
    std::atomic<bool> m_running{false};
    FocusScorer       m_scorer;
    static QImage matToQImage(const cv::Mat &mat);
};
