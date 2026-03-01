#include "FocusAnalyzer.h"

#include <opencv2/imgproc.hpp>

FocusAnalyzer::FocusAnalyzer(const std::string &cascade_dir) {
    face_cascade.load(cascade_dir + "/haarcascade_frontalface_default.xml");
    eye_cascade.load(cascade_dir + "/haarcascade_eye.xml");
}

bool FocusAnalyzer::isLoaded() const {
    return !face_cascade.empty() && !eye_cascade.empty();
}

FrameAnalysis FocusAnalyzer::analyze(const cv::Mat &frame) {
    FrameAnalysis result;

    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(gray, gray);

    std::vector<cv::Rect> face_rects;
    face_cascade.detectMultiScale(gray, face_rects);

    for (const auto &face_rect : face_rects) {
        FaceData face;
        face.rect = face_rect;

        cv::Mat face_roi = gray(face_rect);
        std::vector<cv::Rect> eye_rects;
        eye_cascade.detectMultiScale(face_roi, eye_rects);

        for (auto &eye : eye_rects) {
            eye.x += face_rect.x;
            eye.y += face_rect.y;
        }

        face.eyes      = eye_rects;
        face.eyes_open = !eye_rects.empty();
        result.faces.push_back(face);
    }

    return result;
}

void FocusAnalyzer::drawResults(cv::Mat &frame, const FrameAnalysis &analysis) {
    for (const auto &face : analysis.faces) {
        cv::rectangle(frame, face.rect, cv::Scalar(0, 255, 0), 2);

        for (const auto &eye : face.eyes) {
            cv::Point center(eye.x + eye.width / 2, eye.y + eye.height / 2);
            cv::circle(frame, center, eye.width / 2, cv::Scalar(255, 0, 0), 2);
        }
    }
}
