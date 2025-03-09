configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/version.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/version.h
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
        *.cpp
        *.h
        *.ui
)

qt_add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

set_target_properties(${PROJECT_NAME}
        PROPERTIES
        WIN32_EXECUTABLE TRUE
)

target_link_libraries(${PROJECT_NAME}
        PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

qt_add_resources(${PROJECT_NAME} resources
        PREFIX "/data"
        FILES stratagem_data.json
)

if (WIN32)
    get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")

    add_custom_target(WinDeploy
            COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/deploy"
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/deploy"

            COMMAND ${CMAKE_COMMAND} -E copy
            "$<TARGET_FILE:${PROJECT_NAME}>"
            "${CMAKE_BINARY_DIR}/deploy/"

            COMMAND "${WINDEPLOYQT_EXECUTABLE}"
            --no-translations
            --no-system-d3d-compiler
            --no-quick-import
            --compiler-runtime
            "${CMAKE_BINARY_DIR}/deploy/$<TARGET_FILE_NAME:${PROJECT_NAME}>"

            COMMAND ${CMAKE_COMMAND} -E echo "Deployed application is ready at: ${CMAKE_BINARY_DIR}/deploy/$<TARGET_FILE_NAME:${PROJECT_NAME}>"

            DEPENDS ${PROJECT_NAME}
            COMMENT "Deploying Qt application to ${CMAKE_BINARY_DIR}/deploy"
    )
endif ()