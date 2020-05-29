# Projects
add_subdirectory(App)
add_subdirectory(Library)

if(MSVC)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()

configure_file(${PROJECT_NAME}.h.in ${CMAKE_BINARY_DIR}/Source/Generated/${PROJECT_NAME}.h)
configure_file(Settings.h.in ${CMAKE_BINARY_DIR}/Source/Generated/${PROJECT_NAME}.settings)

message("✶ Verbose Information")
message("Current='${PROJECT_NAME}'")
set(VALUES "")
get_property(VALUES TARGET ${PROJECT_NAME} PROPERTY INCLUDE_DIRECTORIES)
foreach(VALUE ${VALUES})
	message(" - Property[INCLUDE_DIRECTORIES]: ${VALUE}")
endforeach()

message("Current='Omnia'")
set(VALUES "")
get_property(VALUES TARGET Omnia PROPERTY INCLUDE_DIRECTORIES)
foreach(VALUE ${VALUES})
	message(" - Property[INCLUDE_DIRECTORIES]: ${VALUE}")
endforeach()
