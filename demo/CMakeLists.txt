cmake_minimum_required(VERSION 3.1)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

add_executable (imgviewer main.cpp MainWindow.cpp ImageSwitcher.cpp ViewerWidget.cpp)

find_package(Qt5Core)
find_package(Qt5Widgets CONFIG REQUIRED)

target_link_libraries (imgviewer LINK_PUBLIC ImageViewerWidget Qt5::Core Qt5::Widgets)
