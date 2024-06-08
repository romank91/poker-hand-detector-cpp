#pragma once
#include <vector>
#include <memory>
#include "helperClasses.hpp"

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

constexpr static auto MODEL_PATH = "../../app/playingCards.onnx";
constexpr static auto CLASSES_PATH = "../../models/Classes.txt";

struct Detection
{
    int classID;
    std::string className;
    float confidence;
    cv::Rect box;
    cv::Scalar color;
};

class HandDetector
{
    public:
        HandDetector();
        ~HandDetector() = default;

        void handDetectorWorker(FrameQueue &frameQueue);

        inline void setConfig(std::shared_ptr<Config> config)
        {
            config_ = config;
        }
 
    private:
        void init();
        void loadClassesFromFile();
        void loadOnnxNetwork();
        cv::Mat resizeFrame(const cv::Mat &source);
        void runInference(const cv::Mat &image, std::vector<Detection> &detections);

        std::string classesPath;
        std::string netPath;
        std::vector<std::string> classes;
        cv::dnn::Net neuralNet;

        std::shared_ptr<Config> config_{std::make_shared<Config>()};
};