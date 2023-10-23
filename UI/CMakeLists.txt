cmake_minimum_required(VERSION 3.25)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC 0N)
set(CMAKE_AUTOUIC ON)

# project(FON_UI)

find_package(Qt5 REQUIRED Core Gui Widgets)

add_executable(main main.cpp mainwindow.cpp loginDialog.cpp leftWidget.cpp)
target_sources(main PRIVATE include/leftWidget.h include/mainwindow.h include/loginDialog.h)
target_link_libraries(main Qt5::Core Qt5::Gui Qt5::Widgets)
target_include_directories(main PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)

