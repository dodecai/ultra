# Application: Ultra
add_executable (Ultra "App.cpp")

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER Application)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/Source/Library/Source)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/Source/Generated)


target_link_libraries(${PROJECT_NAME} PRIVATE Omnia)
