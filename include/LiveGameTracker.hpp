#include "HandDetector.hpp"
#include "Camera.hpp"
#include <queue>
#include "helperClasses.hpp"


class LiveGameTracker
{
    public:
        LiveGameTracker(const uint32_t devID);
        ~LiveGameTracker() = default;

        void run();

    private:
        Camera camera_;
        HandDetector handDetector_;
        FrameQueue frameQueue; 
};