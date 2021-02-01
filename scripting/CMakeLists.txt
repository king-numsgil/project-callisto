set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

add_subdirectory(math)

add_library(CallistoScripting INTERFACE)

target_link_libraries(CallistoScripting
	PRIVATE
	PUBLIC
	INTERFACE
		CallistoScripting-Math
)
