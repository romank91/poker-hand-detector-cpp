#pragma once

#include <queue>
#include <opencv2/core.hpp>

struct FrameQueue
{
    std::queue<cv::Mat> raw;
    std::queue<cv::Mat> processed;
};