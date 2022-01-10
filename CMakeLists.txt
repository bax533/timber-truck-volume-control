cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME Timber-Volume-Control)
#set(PROJECT_NAME_INTERFACE timber_volume_control)

project(${PROJECT_NAME} C CXX)

#add_definitions(-DINTERFACE_EXPORT)

file(GLOB_RECURSE HEADER_FILES "include/*.hpp" "include/*.h")
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")

find_package(ZED 3 REQUIRED)
find_package(CUDA REQUIRED)
find_package(OpenCV REQUIRED)

message("CUDA VERSION: ${ZED_CUDA_VERSION}")

add_executable(${PROJECT_NAME} src/main.cpp ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${EIGEN3_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${ZED_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${CUDA_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories( ${OpenCV_INCLUDE_DIRS} )

target_link_directories(${PROJECT_NAME} PUBLIC ${ZED_LIBRARY_DIR})
target_link_directories(${PROJECT_NAME} PUBLIC ${CUDA_LIBRARY_DIRS})

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" -O3 -std=c++17)

#SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
#set(CMAKE_POSITION_INDEPENDENT_CODE ON)
#add_definitions(-std=c++17 -O3 -fPIC -fpermissive)

#add_library(${PROJECT_NAME_INTERFACE} SHARED ${SOURCE_FILES} ${HEADER_FILES})

exec_program(find ARGS "/usr/lib/ -type f -name libnvcuvid*" OUTPUT_VARIABLE CUVID_LIBRARY)

target_link_libraries(${PROJECT_NAME}#"${PROJECT_NAME_INTERFACE}"
    ${ZED_LIBRARIES}
    ${CUDA_CUDA_LIBRARY}
#    gomp
#    pthread
#    usb
    ${CUVID_LIBRARY}
    ${OpenCV_LIBS}
)
