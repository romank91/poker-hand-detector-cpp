include(FetchContent)
find_package( OpenCV REQUIRED )

FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG 0.8.0
)
FetchContent_GetProperties(yaml-cpp)

if(NOT yaml-cpp_POPULATED)
  message(STATUS "Fetching yaml-cpp...")
  FetchContent_Populate(yaml-cpp)
  add_subdirectory(${yaml-cpp_SOURCE_DIR} ${yaml-cpp_BINARY_DIR})
endif()

add_library(Camera STATIC Camera.cpp)
add_library(HandDetector STATIC HandDetector.cpp)
add_library(LiveGameTracker STATIC LiveGameTracker.cpp)

target_include_directories(Camera PUBLIC ${OpenCV_INCLUDE_DIRS} "../include" "../utils")
target_include_directories(HandDetector PUBLIC ${OpenCV_INCLUDE_DIRS} "../include" "../utils")
target_include_directories(LiveGameTracker PUBLIC "../include" "../utils")
target_include_directories(HandDetector PUBLIC "../utils")

target_link_libraries(Camera ${OpenCV_LIBS})
target_link_libraries(HandDetector ${OpenCV_LIBS} )
target_link_libraries(LiveGameTracker Camera HandDetector yaml-cpp::yaml-cpp)