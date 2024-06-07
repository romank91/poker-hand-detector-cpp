

#include "LiveGameTracker.hpp"
#include <iostream>
#include <string>

int main(int argc, char** argv)
{
 
    auto liveGameTracker = LiveGameTracker();
    liveGameTracker.run();

    std::cout << "Finishing" << std::endl;
    return 0;
}