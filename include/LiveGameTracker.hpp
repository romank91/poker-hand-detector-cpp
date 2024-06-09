#include "HandDetector.hpp"
#include "Camera.hpp"
#include <queue>
#include <memory>
#include "helperClasses.hpp"


class LiveGameTracker
{
    public:
        LiveGameTracker();
        ~LiveGameTracker() = default;
        
        void run();

    private:
        void init();
        void loadConfig();
        void setRootPath();

        std::shared_ptr<Config> config_{std::make_shared<Config>()};
        Camera camera_;
        HandDetector handDetector_;
        FrameQueue frameQueue_; 
};