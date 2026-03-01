find_package(OpenCV REQUIRED core imgproc videoio objdetect)

add_subdirectory(algorithm)
add_subdirectory(UI)

add_executable(FON)
target_sources(FON PRIVATE
    main.cpp
    )
# algorithm and FON_UI already bring their OpenCV dependencies transitively
target_link_libraries(FON PRIVATE algorithm FON_UI)
target_include_directories(FON PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)

# Locate OpenCV Haar cascade files (cross-platform)
# Covers: Windows MSVC prebuilt, Windows custom/MinGW build, Linux system install, Linux source build
find_path(OPENCV_HAARCASCADES_DIR
    NAMES haarcascade_frontalface_default.xml
    HINTS
        "${OpenCV_DIR}/../../../etc/haarcascades"
        "${OpenCV_DIR}/../../etc/haarcascades"
        "${OpenCV_DIR}/../../../data/haarcascades"
        "${OpenCV_DIR}/../../data/haarcascades"
        "${OpenCV_DIR}/../data/haarcascades"
        "${OpenCV_DIR}/data/haarcascades"
        "${OpenCV_DIR}/../../share/opencv4/haarcascades"
        "${OpenCV_DIR}/../../share/OpenCV/haarcascades"
        "/usr/share/opencv4/haarcascades"
        "/usr/share/opencv/haarcascades"
    NO_DEFAULT_PATH
)

if(OPENCV_HAARCASCADES_DIR)
    add_custom_command(TARGET FON POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:FON>/data/haarcascades"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${OPENCV_HAARCASCADES_DIR}/haarcascade_frontalface_default.xml"
            "$<TARGET_FILE_DIR:FON>/data/haarcascades/haarcascade_frontalface_default.xml"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${OPENCV_HAARCASCADES_DIR}/haarcascade_eye.xml"
            "$<TARGET_FILE_DIR:FON>/data/haarcascades/haarcascade_eye.xml"
        COMMENT "Copying OpenCV haarcascades to build directory"
    )
    message(STATUS "OpenCV haarcascades found: ${OPENCV_HAARCASCADES_DIR}")
else()
    message(WARNING "OpenCV haarcascades not found automatically. "
        "Place cascade files manually at: <build>/data/haarcascades/")
endif()
