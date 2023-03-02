cmake_minimum_required(VERSION 3.24 FATAL_ERROR)
project(libtorch_example)

set(CMAKE_CXX_STANDARD 14)

# --- Torch libraries
set(CMAKE_PREFIX_PATH "../../../../local/cpp/libtorch/")
find_package(Torch CONFIG REQUIRED)

# --- Boost libraries
set(CMAKE_PREFIX_PATH "/opt/homebrew/Cellar/boost/1.81.0_1/")
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# -- build the target
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS} ${BOOST_CXX_FLAGS}")
add_executable(libtorch_example src/main.cpp)
target_link_libraries(libtorch_example PRIVATE ${TORCH_LIBRARIES} ${Boost_LIBRARIES})
set_property(TARGET libtorch_example PROPERTY CXX_STANDARD 14)
