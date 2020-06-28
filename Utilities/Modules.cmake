
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
