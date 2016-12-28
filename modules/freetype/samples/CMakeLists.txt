CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

set(name "freetype2")
project(freetype2_cpp_samples)

#SET(OpenCV_DIR /path/to/your/opencv/installation)

# packages
find_package(OpenCV REQUIRED) # http://opencv.org

if(PKG_CONFIG_FOUND)
  pkg_search_module(FREETYPE freetype2)
  pkg_search_module(HARFBUZZ harfbuzz)
endif()

if( FREETYPE_FOUND AND HARFBUZZ_FOUND )
  message(STATUS "no example because FREETYPE and/or HARFBUZZ are not FOUND")
else()
  add_executable(freetype_demo freetype_demo.cpp)
  target_link_libraries(freetype_demo 
                        opencv_core opencv_imgcodecs opencv_freetype)
  add_executable(freetype_simple freetype_simple.cpp)
  target_link_libraries(freetype_simple
                        opencv_core opencv_imgcodecs opencv_freetype)
endif()
