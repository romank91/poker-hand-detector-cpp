#include "HandDetector.hpp"
#include "Camera.hpp"
#include <queue>
#include "helperClasses.hpp"


class LiveGameTracker
{
    public:
        LiveGameTracker() = default;
        ~LiveGameTracker() = default;

        void run();
        void videoFeedWorker();

    private:
        Camera camera_;
        HandDetector handDetector_;
        FrameQueue frameQueue; 
};