cmake_minimum_required(VERSION 2.8)

PROJECT(DICOMVolumeRendering)

find_package(VTK 8.0.0 REQUIRED)

include(${VTK_USE_FILE})

file(GLOB SOURCES *.cpp)

add_executable(DICOMVolumeRendering MACOSX_BUNDLE main.cpp)

target_link_libraries(DICOMVolumeRendering ${VTK_LIBRARIES})