#pragma once
#include <vector>
#include <memory>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "helperClasses.hpp"

class HandDetector
{
    public:
        HandDetector() = default;
        ~HandDetector() = default;

        void init();
        void handDetectorWorker(FrameQueue &frameQueue);

        inline void setConfig(std::shared_ptr<Config> config)
        {
            config_ = config;
        }
 
    private:
        void loadClassesFromFile();
        void loadOnnxNetwork();
        cv::Mat resizeFrame(const cv::Mat &source);
        void runInference(const cv::Mat &image, std::vector<Detection> &detections);

        std::vector<std::string> classes_;
        cv::dnn::Net neuralNet_;
        std::shared_ptr<Config> config_{std::make_shared<Config>()};
};