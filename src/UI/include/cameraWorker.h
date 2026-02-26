#pragma once

#include <QImage>
#include <QThread>
#include <atomic>

#include "CameraCapture.h"

class CameraWorker : public QThread {
    Q_OBJECT
public:
    explicit CameraWorker(QObject *parent = nullptr);
    ~CameraWorker() override;

signals:
    void frameReady(const QImage &frame);

public slots:
    void startCapture();
    void stopCapture();

protected:
    void run() override;

private:
    std::atomic<bool> m_running{false};
    static QImage matToQImage(const cv::Mat &mat);
};
