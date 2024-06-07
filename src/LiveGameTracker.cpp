#include <thread>
#include "LiveGameTracker.hpp"


LiveGameTracker::LiveGameTracker(const uint32_t devID) 
{
    // Init camera
    camera_.init(devID);
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
        if (!frameQueue.processed.empty())
        {
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