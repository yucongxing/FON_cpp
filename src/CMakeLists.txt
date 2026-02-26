find_package(OpenCV REQUIRED imgproc core videoio)

add_subdirectory(algorithm)
add_subdirectory(UI)

add_executable(FON)
target_sources(FON PRIVATE
    main.cpp
    )
target_link_libraries(FON PRIVATE opencv_imgproc opencv_core opencv_videoio algorithm FON_UI)
target_include_directories(FON PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)

# Auto-copy OpenCV haarcascades to executable directory after build
foreach(_dir
    "${OpenCV_DIR}/../../../data/haarcascades"
    "${OpenCV_DIR}/../../share/opencv4/haarcascades"
    "${OpenCV_DIR}/../../share/OpenCV/haarcascades"
    "${OpenCV_DIR}/../../../etc/haarcascades"
)
    if(EXISTS "${_dir}/haarcascade_frontalface_default.xml")
        get_filename_component(_cascade_src "${_dir}" ABSOLUTE)
        break()
    endif()
endforeach()

if(DEFINED _cascade_src)
    add_custom_command(TARGET FON POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:FON>/data/haarcascades"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${_cascade_src}/haarcascade_frontalface_default.xml"
            "$<TARGET_FILE_DIR:FON>/data/haarcascades/haarcascade_frontalface_default.xml"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${_cascade_src}/haarcascade_eye.xml"
            "$<TARGET_FILE_DIR:FON>/data/haarcascades/haarcascade_eye.xml"
        COMMENT "Copying OpenCV haarcascades to build directory"
    )
    message(STATUS "OpenCV haarcascades found: ${_cascade_src}")
else()
    message(WARNING "OpenCV haarcascades not found automatically. "
        "Place cascade files manually at: <build>/data/haarcascades/")
endif()
