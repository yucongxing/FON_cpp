#pragma once

#include <opencv2/objdetect.hpp>
#include <opencv2/core.hpp>
#include <string>
#include <vector>

struct FaceData {
    cv::Rect rect;
    std::vector<cv::Rect> eyes;
    bool eyes_open = false;
};

struct FrameAnalysis {
    std::vector<FaceData> faces;
};

class FocusAnalyzer {
public:
    explicit FocusAnalyzer(const std::string &cascade_dir);
    bool isLoaded() const;
    FrameAnalysis analyze(const cv::Mat &frame);
    void drawResults(cv::Mat &frame, const FrameAnalysis &analysis);

private:
    cv::CascadeClassifier face_cascade;
    cv::CascadeClassifier eye_cascade;
};
