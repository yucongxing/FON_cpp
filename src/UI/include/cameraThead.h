#pragma once
#include <QObject>

#include "CameraCapture.h"

class CameraThead : public QObject {
    Q_OBJECT
public:
    CameraThead(QObject *parent = nullptr);
signals:
    void refreshFrameSignal(const QImage &img);
public slots:
    void refreshFrame();

private:
    CameraCapture cap;
    static QImage cvmat2Qimage(const cv::Mat &mat);
};
