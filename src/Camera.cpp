#include "Camera.hpp"
#include <iostream>
#include <thread>
#include <opencv2/highgui.hpp>


void Camera::captureWorker(FrameQueue &frameQueue)
{
    cv::Mat frame;
    int counter = 0;

    while (true)
    {   
        cap_.read(frame);

        if (frame.empty()) 
        {
            std::cerr << "ERROR! blank frame grabbed\n";
            break;
        }

        if (frameQueue.raw.empty())
        {
            frameQueue.raw.push(frame);
        }

        counter++;
        // std::cout << counter << std::endl;
    }
}

void Camera::init()
{ 
    while (!cap_.isOpened()) 
    { 
        std::cout << "Please enter video device ID: ";
        std::cin >> deviceID_;

        cap_.open(deviceID_, API_ID);

        if (!cap_.isOpened()) std::cerr << "ERROR! Unable to open camera\n";
        else break;   
    }
}



