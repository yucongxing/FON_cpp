add_library(algorithm)

target_sources(algorithm PRIVATE
    CameraCapture.cpp    include/CameraCapture.h
    FocusAnalyzer.cpp    include/FocusAnalyzer.h)

target_link_libraries(algorithm PUBLIC opencv_core opencv_videoio opencv_imgproc opencv_objdetect)
target_include_directories(algorithm PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
