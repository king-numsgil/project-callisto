set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

corrade_add_resource(CALLISTO_SHADERS_RCS resource/shaders/resource.conf)

add_executable(CallistoGame
	${CALLISTO_SHADERS_RCS}

	source/main.cpp
	source/ImContext.cpp
	source/ImContext.hpp
	source/Components.hpp
	source/ScriptContext.cpp
	source/ScriptContext.hpp
	source/HexGrid.hpp
	source/HexGrid.cpp
)

set_target_properties(CallistoGame
	PROPERTIES ${DEFAULT_PROJECT_OPTIONS}
)

target_include_directories(CallistoGame
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/source
		${CMAKE_CURRENT_BINARY_DIR}/source
	PUBLIC
		${DEFAULT_INCLUDE_DIRECTORIES}
)

target_link_libraries(CallistoGame
	PRIVATE
		Json EnTT sol2 ImGUI
		CallistoScripting

		Corrade::Containers
		Corrade::Utility
		Corrade::Main

		MagnumPlugins::TinyGltfImporter
		MagnumPlugins::StbImageImporter
		MagnumPlugins::StbTrueTypeFont

		Magnum::Magnum
		Magnum::GlfwApplication
		Magnum::Primitives
		Magnum::MeshTools
		Magnum::Shaders
		Magnum::Trade
		Magnum::Text
		Magnum::GL
	PUBLIC
		${DEFAULT_LIBRARIES}
)

target_compile_definitions(CallistoGame
	PRIVATE
	PUBLIC
		${DEFAULT_COMPILE_DEFINITIONS}
)

target_compile_options(CallistoGame
	PRIVATE
	PUBLIC
		${DEFAULT_COMPILE_OPTIONS}
)

target_link_libraries(CallistoGame
	PRIVATE
	PUBLIC
		${DEFAULT_LINKER_OPTIONS}
)
