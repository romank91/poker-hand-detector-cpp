#pragma once
#include <cstdint>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <queue>
#include "helperClasses.hpp"

constexpr static auto API_ID = cv::CAP_ANY;     // 0 = autodetect default API
constexpr static auto CAMERA_DEVICE_ID = 0;

class Camera 
{
    public:
        Camera();
        ~Camera() = default;

        void captureWorker(FrameQueue &frameQueue);

    private:
        bool init();

        cv::VideoCapture cap;       
};