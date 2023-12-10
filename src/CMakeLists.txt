find_package(OpenCV REQUIRED imgproc core videoio)

add_subdirectory(algorithm)
add_subdirectory(UI)

add_executable(FON)
target_sources(FON PRIVATE
    main.cpp
    )
target_link_libraries(FON PRIVATE opencv_imgproc opencv_core opencv_videoio algorithm FON_UI)
target_include_directories(FON PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
