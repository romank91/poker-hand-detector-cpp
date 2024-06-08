#pragma once

#include <queue>
#include <opencv2/core.hpp>

struct FrameQueue
{
    std::queue<cv::Mat> raw;
    std::queue<cv::Mat> processed;
};

struct Config
{
    bool cuda_enabled;
    uint32_t inputWidth;
    uint32_t inputHeight;
    float scoreThreshold;
    float nmsThreshold;
    float confidenceThreshold;
};