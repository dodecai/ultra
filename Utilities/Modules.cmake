
# Simple 'add_subdirectory' extension with a nice message bevore
macro(add_subdirectoryX argument)
	message("------------------------------------------------------------------------------------------------")
	message("Project: ${argument}")
	message("------------------------------------------------------------------------------------------------")
	add_subdirectory(${argument})
endmacro()

# Simple 'target_link_library' extension with a nice message bevore
macro(target_link_librariesX Target Scope Libraries)
	message("  ҉ Linking libraries to target '${Target}'")
	foreach(Library ${Libraries})
		message("    - ${Library}")
		target_link_libraries(${Target} ${Scope} ${Library})
	endforeach()
endmacro()

# Simple 'message' extension to output property list of the given target
macro(message_property project property)
	message("  ○ Property [${property}]")

	set(counter 1)
	set(values "")
	
	get_property(values TARGET ${project} PROPERTY ${property})
	foreach(value ${values})
		message("    - ${counter}: ${value}")
		MATH(EXPR counter "${counter}+1")
	endforeach()
endmacro()



# Applies CMAKE_CXX_FLAGS to all targets in the current CMake directory.
# Note: After this operation, CMAKE_CXX_FLAGS is cleared.
macro(apply_compiler_flags_global)
    separate_arguments(_global_cxx_flags_list UNIX_COMMAND ${CMAKE_CXX_FLAGS})
    get_property(_targets DIRECTORY PROPERTY BUILDSYSTEM_TARGETS)
    foreach(_target ${_targets})
        target_compile_options(${_target} PUBLIC ${_global_cxx_flags_list})
    endforeach()
    unset(CMAKE_CXX_FLAGS)
    set(_flag_sync_required TRUE)
endmacro()

# Removes the specified compile flag from the specified target.
macro(remove_target_compiler_flag _target _flag)
    get_target_property(_target_cxx_flags ${_target} COMPILE_OPTIONS)
    if(_target_cxx_flags)
        list(REMOVE_ITEM _target_cxx_flags ${_flag})
        set_target_properties(${_target} PROPERTIES COMPILE_OPTIONS "${_target_cxx_flags}")
    endif()
endmacro()
