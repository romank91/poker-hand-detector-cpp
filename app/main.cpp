#include <iostream>
#include <string>
#include "LiveGameTracker.hpp"

int main(int argc, char** argv)
{
    auto liveGameTracker = LiveGameTracker();
    liveGameTracker.run();

    std::cout << "Finishing...\n";
    return 0;
}