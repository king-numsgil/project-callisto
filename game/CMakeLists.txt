set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

corrade_add_resource(UMBRIEL_IMGUI_RCS resource/imgui/resource.conf)
corrade_add_resource(UMBRIEL_SPRITES_RCS resource/sprites/resource.conf)

add_executable(UmbrielGame
		include/umbriel/Components.hpp
		include/umbriel/GameState.hpp
		include/umbriel/State.hpp
		include/umbriel/Types.hpp
		include/umbriel/Log.hpp
		source/Components.cpp
		source/GameState.cpp
		source/State.cpp
		source/main.cpp
		${UMBRIEL_SPRITES_RCS}

		include/umbriel/ImContext.hpp
		source/ImContext.cpp
		${UMBRIEL_IMGUI_RCS}
)

set_target_properties(UmbrielGame
	PROPERTIES ${DEFAULT_PROJECT_OPTIONS}
)

target_include_directories(UmbrielGame
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/include
		${CMAKE_CURRENT_BINARY_DIR}/include
	PUBLIC
		${DEFAULT_INCLUDE_DIRECTORIES}
)

target_link_libraries(UmbrielGame
	PRIVATE
		Json EnTT ImGUI Chipmunk

		Corrade::Containers
		Corrade::Utility
		Corrade::Main

		MagnumPlugins::StbImageImporter

		Magnum::Magnum
		Magnum::GlfwApplication
		Magnum::SceneGraph
		Magnum::Primitives
		Magnum::MeshTools
		Magnum::Shaders
		Magnum::Trade
		Magnum::GL
	PUBLIC
		${DEFAULT_LIBRARIES}
)

target_compile_definitions(UmbrielGame
	PRIVATE
	PUBLIC
		${DEFAULT_COMPILE_DEFINITIONS}
)

target_compile_options(UmbrielGame
	PRIVATE
	PUBLIC
		${DEFAULT_COMPILE_OPTIONS}
)

target_link_libraries(UmbrielGame
	PRIVATE
	PUBLIC
		${DEFAULT_LINKER_OPTIONS}
)
