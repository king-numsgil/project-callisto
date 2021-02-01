set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

add_library(CallistoScripting-Math SHARED
		include/callisto/binding-math.hpp
		include/callisto/binding/vec2.hpp
		include/callisto/binding/vec3.hpp
		include/callisto/binding/vec4.hpp
		include/callisto/binding/mat.hpp
		include/callisto/binding/angle.hpp
		include/callisto/binding/complex.hpp
		include/callisto/binding/quat.hpp
		source/vec2.cpp
		source/vec3.cpp
		source/vec4.cpp
		source/mat.cpp
		source/angle.cpp
		source/complex.cpp
		source/quat.cpp
)

generate_export_header(CallistoScripting-Math
		EXPORT_MACRO_NAME SCRIPTING_API
		EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/include/ScriptingMath_export.h
)

set_target_properties(CallistoScripting-Math
	PROPERTIES ${DEFAULT_PROJECT_OPTIONS}
)

target_include_directories(CallistoScripting-Math
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/include
		${CMAKE_CURRENT_BINARY_DIR}/include
	PUBLIC
		${DEFAULT_INCLUDE_DIRECTORIES}
	INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
)

target_link_libraries(CallistoScripting-Math
	PRIVATE
		Json sol2

		Corrade::Containers
		Corrade::Utility

		Magnum::Magnum
		Magnum::Primitives
		Magnum::MeshTools
		Magnum::Shaders
		Magnum::Trade
		Magnum::Text
		Magnum::GL
	PUBLIC
		${DEFAULT_LIBRARIES}
)

target_compile_definitions(CallistoScripting-Math
	PRIVATE
	PUBLIC
		${DEFAULT_COMPILE_DEFINITIONS}
)

target_compile_options(CallistoScripting-Math
	PRIVATE
	PUBLIC
		${DEFAULT_COMPILE_OPTIONS}
)

target_link_libraries(CallistoScripting-Math
	PRIVATE
	PUBLIC
		${DEFAULT_LINKER_OPTIONS}
)
