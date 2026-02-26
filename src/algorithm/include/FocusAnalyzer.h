#pragma once

#include <opencv2/objdetect.hpp>
#include <opencv2/core.hpp>
#include <string>
#include <vector>

struct FrameAnalysis {
    bool has_face = false;
    std::vector<cv::Rect> faces;
};

class FocusAnalyzer {
public:
    explicit FocusAnalyzer(const std::string &cascade_path);
    bool isLoaded() const;
    FrameAnalysis analyze(const cv::Mat &frame);
    void drawResults(cv::Mat &frame, const FrameAnalysis &analysis);

private:
    cv::CascadeClassifier face_cascade;
};
