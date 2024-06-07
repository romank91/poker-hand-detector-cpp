

#include "LiveGameTracker.hpp"
#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    uint32_t deviceID;
    std::cout << "Please enter video device ID: ";
    std::cin >> deviceID;

    auto liveGameTracker = LiveGameTracker(deviceID);
    liveGameTracker.run();

    std::cout << "Finishing...\n";
    return 0;
}