#pragma once
#include <cstdint>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <queue>
#include "helperClasses.hpp"

constexpr static auto API_ID = cv::CAP_ANY;     // 0 = autodetect default API

class Camera 
{
    public:
        Camera() = default;
        ~Camera() = default;

        bool init(const uint32_t devID);
        void captureWorker(FrameQueue &frameQueue);

    private:
        cv::VideoCapture cap;       
};