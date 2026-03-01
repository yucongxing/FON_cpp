cmake_minimum_required(VERSION 3.25)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC OFF)
set(CMAKE_AUTOUIC ON)

# project(FON_UI)

find_package(Qt5 REQUIRED Core Gui Widgets)

add_library(FON_UI)

target_sources(FON_UI
    PRIVATE
    mainwindow.cpp
    loginDialog.cpp
    leftWidget.cpp
    cameraWorker.cpp
    include/leftWidget.h
    include/mainwindow.h
    include/loginDialog.h
    include/cameraWorker.h)
target_link_libraries(FON_UI PUBLIC Qt5::Core Qt5::Gui Qt5::Widgets algorithm)
target_include_directories(FON_UI PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

# Ensure Chinese string literals are compiled as UTF-8 on MSVC
if(MSVC)
    target_compile_options(FON_UI PRIVATE /utf-8)
endif()

