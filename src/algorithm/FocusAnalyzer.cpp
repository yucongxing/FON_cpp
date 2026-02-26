#include "FocusAnalyzer.h"

#include <opencv2/imgproc.hpp>

FocusAnalyzer::FocusAnalyzer(const std::string &cascade_path) {
    face_cascade.load(cascade_path);
}

bool FocusAnalyzer::isLoaded() const {
    return !face_cascade.empty();
}

FrameAnalysis FocusAnalyzer::analyze(const cv::Mat &frame) {
    FrameAnalysis result;
    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(gray, gray);
    face_cascade.detectMultiScale(gray, result.faces);
    result.has_face = !result.faces.empty();
    return result;
}

void FocusAnalyzer::drawResults(cv::Mat &frame, const FrameAnalysis &analysis) {
    for (const auto &face : analysis.faces) {
        cv::rectangle(frame, face, cv::Scalar(0, 255, 0), 2);
    }
}
