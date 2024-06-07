#pragma once
#include <vector>
#include "helperClasses.hpp"

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

constexpr static float INPUT_WIDTH = 640.0;
constexpr static float INPUT_HEIGHT = 640.0;
constexpr static float SCORE_THRESHOLD = 0.2;
constexpr static float NMS_THRESHOLD = 0.4;
constexpr static float CONFIDENCE_THRESHOLD = 0.5;

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
        
    private:
        void init();
        void loadClassesFromFile();
        void loadOnnxNetwork(const bool cudaEnabled = false);
        cv::Mat resizeFrame(const cv::Mat &source);
        std::vector<Detection> runInference(const cv::Mat &image);

        std::string classesPath;
        std::string netPath;
        std::vector<std::string> classes;
        cv::dnn::Net neuralNet;
};