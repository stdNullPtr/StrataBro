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