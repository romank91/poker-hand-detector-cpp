#pragma once
#include <cstdint>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <queue>
#include <memory>
#include "helperClasses.hpp"

constexpr static auto API_ID = cv::CAP_ANY;     // 0 = autodetect default API

class Camera 
{
    public:
        Camera() = default;
        ~Camera() = default;

        void init();  
        void captureWorker(FrameQueue &frameQueue);

        inline void setConfig(std::shared_ptr<Config> config)
        {
            config_ = config;
        }

    private:
        std::shared_ptr<Config> config_{std::make_shared<Config>()};
        uint32_t deviceID_;
        cv::VideoCapture cap_;    

         
};