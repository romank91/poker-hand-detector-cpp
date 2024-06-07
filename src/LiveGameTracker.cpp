#include "LiveGameTracker.hpp"
#include <thread>

// void LiveGameTracker::videoFeedWorker()
// {
//     while (true)
//     {
//         cv::Mat frame = frameQueue.front();
//         cv::imshow("Live", frame);
        
//         if (cv::waitKey(5) >= 0)
//         {
//             break;
//         }

//         frameQueue.pop()
//     }
// }

void LiveGameTracker::run()
{
    // Start capture in a new thread
    auto captureThread = std::thread(&Camera::captureWorker, camera_, std::ref(frameQueue));

    // Start hand detector in a new thread
    auto handDetectorThread = std::thread(&HandDetector::handDetectorWorker, handDetector_, std::ref(frameQueue));

    cv::Mat frame;

    while (true)
    {
        if (!frameQueue.processed.empty())
        {
            frame = frameQueue.processed.front();
            cv::imshow("Live", frame);
            frameQueue.processed.pop();
        }
        
        if (cv::waitKey(5) >= 0)
        {
            break;
        }
    }

    if (captureThread.joinable())
    {
        captureThread.join();
    }

    if (handDetectorThread.joinable())
    {
        handDetectorThread.join();
    }   
}