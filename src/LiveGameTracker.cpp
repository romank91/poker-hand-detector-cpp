#include <thread>
#include <chrono>
#include <filesystem>
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
    handDetector_.init();
}

void LiveGameTracker::setRootPath()
{
    // Set root path
    const auto filePath = std::string(__FILE__);
    const size_t idx = filePath.find("/src");

    if (idx != std::string::npos)
    {
        config_->rootPath = std::filesystem::path(filePath.substr(0, idx));
    }
    else 
    {
        std::cerr << "Error! Wrong path specified!\n";
    }
}

void LiveGameTracker::loadConfig()
{
    setRootPath();

    const std::filesystem::path configPath = config_->rootPath / std::filesystem::path("utils/config.yaml");
    YAML::Node config = YAML::LoadFile(configPath);

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
    auto captureThread = std::thread(&Camera::captureWorker, &camera_, std::ref(frameQueue_));
    auto handDetectorThread = std::thread(&HandDetector::handDetectorWorker, &handDetector_, std::ref(frameQueue_));

    // Set variables to monitor FPS
    auto startTime = std::chrono::high_resolution_clock::now();
    auto curTime = startTime;
    int frameCount = 0;
    float fps;

    // Process frame queue
    cv::Mat frame;

    while (true)
    {
        // Show oldest annotated frame in queue
        if (!frameQueue_.processed.empty())        
        {
            frame = frameQueue_.processed.front();
            cv::imshow("Live", frame);
            frameQueue_.processed.pop();
            frameCount++;
        }

        // TODO: Add functionality to exit loop
        if (cv::waitKey(5) >= 0) break;
    
        if (frameCount >= 30)
        {
            curTime = std::chrono::high_resolution_clock::now();
            fps = frameCount * 1000.0f / std::chrono::duration_cast<std::chrono::milliseconds>(curTime - startTime).count();

            frameCount = 0;
            startTime = std::chrono::high_resolution_clock::now();

            std::cout << "FPS: " << fps << std::endl;
        }         
    }

    // Join threads
    if (captureThread.joinable()) captureThread.join();
    if (handDetectorThread.joinable()) handDetectorThread.join();
}