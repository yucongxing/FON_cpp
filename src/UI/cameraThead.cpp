#include "cameraThead.h"

#include <QImage>

CameraThead::CameraThead(QObject *parent) : QObject(parent) {}

void CameraThead::refreshFrame() {
    cv::Mat img = cap.getFrame();
    emit(refreshFrameSignal(cvmat2Qimage(img)));
}

QImage CameraThead::cvmat2Qimage(const cv::Mat &mat) {
    QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
    return image.rgbSwapped();
}
