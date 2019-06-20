cmake_minimum_required(VERSION 3.1)

project(imageviewer)

set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED true)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -fPIC")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Core)
find_package(Qt5Widgets CONFIG REQUIRED)

# Build
set(sources
  src/ImageViewer.cpp
  src/FullScreenWindow.cpp
  src/ImgViewerWidget.cpp 
)

set(headers
  include/ImgViewerWidget.h
)

set(library_target ImageViewerWidget)
add_library(${library_target} SHARED ${sources} ${headers})
target_link_libraries (${library_target} LINK_PUBLIC Qt5::Core Qt5::Widgets)
install(TARGETS ${library_target} LIBRARY DESTINATION lib)

target_include_directories(${library_target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Demo app
set(IMGVIEWER_BUILD_DEMO OFF CACHE BOOL "Build ImgViewer demo app")
if(${IMGVIEWER_BUILD_DEMO})
  add_subdirectory(demo)
endif()
