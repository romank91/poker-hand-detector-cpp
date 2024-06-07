add_library(Camera STATIC Camera.cpp)
add_library(HandDetector STATIC HandDetector.cpp)
add_library(LiveGameTracker STATIC LiveGameTracker.cpp)

find_package( OpenCV REQUIRED )

target_include_directories(Camera PUBLIC 
    ${OpenCV_INCLUDE_DIRS} 
    "../include"
    "../utils")

target_include_directories(HandDetector PUBLIC 
    ${OpenCV_INCLUDE_DIRS} 
    "../include"
    "../utils")

target_include_directories(LiveGameTracker PUBLIC 
    "../include"
    "../utils")

target_link_directories(HandDetector PUBLIC
    "../utils")

target_link_libraries(Camera ${OpenCV_LIBS})
target_link_libraries(HandDetector ${OpenCV_LIBS} )
target_link_libraries(LiveGameTracker Camera HandDetector)