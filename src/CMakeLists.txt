cmake_minimum_required(VERSION 2.8)
project( OCR )
find_package( OpenCV )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_definitions(-std=c++11)
set(ocr_source_files main.cpp feature_extraction.cpp feature_extraction.hpp)
add_executable( OCR ${ocr_source_files})
target_link_libraries( OCR ${OpenCV_LIBS})