#include <thread>
#include "LiveGameTracker.hpp"
#include <yaml-cpp/yaml.h>

LiveGameTracker::LiveGameTracker()
{
    init();
}

void LiveGameTracker::init()
{
    loadConfig();
    camera_.init();
}

void LiveGameTracker::loadConfig()
{
    YAML::Node config = YAML::LoadFile("../../utils/config.yaml");

    config_->cuda_enabled = config["yolo"]["cuda_enabled"].as<bool>();
    config_->inputWidth = config["yolo"]["input_width"].as<uint32_t>();
    config_->inputHeight = config["yolo"]["input_height"].as<uint32_t>();
    config_->scoreThreshold = config["yolo"]["score_threshold"].as<float>();
    config_->nmsThreshold = config["yolo"]["nms_threshold"].as<float>();
    config_->confidenceThreshold = config["yolo"]["confidence_threshold"].as<float>();

    camera_.setConfig(config_);
    handDetector_.setConfig(config_);
}

void LiveGameTracker::run()
{
    // TODO: replace threads by jthreads
    // Start threads
    auto captureThread = std::thread(&Camera::captureWorker, camera_, std::ref(frameQueue));
    auto handDetectorThread = std::thread(&HandDetector::handDetectorWorker, handDetector_, std::ref(frameQueue));

    // Process frame queue
    cv::Mat frame;

    while (true)
    {
        // Show oldest annotated frame in queue
        if (!frameQueue.processed.empty())        {
            frame = frameQueue.processed.front();
            cv::imshow("Live", frame);
            frameQueue.processed.pop();
        }

        // TODO: Add functionality to exit loop
        if (cv::waitKey(5) >= 0)
        {
            break;
        }
    }

    // Join threads
    if (captureThread.joinable()) captureThread.join();
    if (handDetectorThread.joinable()) handDetectorThread.join();
}