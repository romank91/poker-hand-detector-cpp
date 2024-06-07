#include "Camera.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <opencv2/highgui.hpp>

void Camera::captureWorker(FrameQueue &frameQueue)
{
    auto startTime = std::chrono::high_resolution_clock::now();
    auto curTime = startTime;
    int frameCount = 0;
    float fps;
    cv::Mat frame;

    while (true)
    {   
        cap.read(frame);

        if (frame.empty()) 
        {
            std::cerr << "ERROR! blank frame grabbed\n";
            break;
        }

        if (frameQueue.raw.empty())
        {
            frameQueue.raw.push(frame);
        }

        frameCount++;

        if (frameCount >= 30)
        {
            curTime = std::chrono::high_resolution_clock::now();
            fps = frameCount * 1000.0f / std::chrono::duration_cast<std::chrono::milliseconds>(curTime - startTime).count();

            frameCount = 0;
            startTime = std::chrono::high_resolution_clock::now();

            std::cout << "FPS: " << fps << std::endl;
        }    
    }
}

bool Camera::init(const uint32_t devID)
{
    cap.open(devID, API_ID);

    if (!cap.isOpened()) 
    {
        std::cerr << "ERROR! Unable to open camera\n";
        return false;
    }
    
    return true;
}



