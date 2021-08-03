set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

corrade_add_resource(CALLISTO_SHADERS_RCS resource/shaders/resource.conf)

add_executable(CallistoGame
	${CALLISTO_SHADERS_RCS}

	source/main.cpp
	source/ImContext.cpp
	source/ImContext.hpp
	source/Components.hpp

	source/hex/all.hpp
	source/hex/Coords.cpp
	source/hex/Coords.hpp
	source/hex/Tile.cpp
	source/hex/Tile.hpp
	source/hex/Grid.cpp
	source/hex/Grid.hpp
	source/hex/Feature.cpp
	source/hex/Feature.hpp
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
		Json EnTT ImGUI

		Corrade::Containers
		Corrade::Utility
		Corrade::Main

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

add_dependencies(CallistoGame
		MagnumPlugins::TinyGltfImporter
		MagnumPlugins::StbImageImporter
		MagnumPlugins::StbTrueTypeFont
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
