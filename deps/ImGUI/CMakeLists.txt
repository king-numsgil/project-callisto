add_library(ImGUI STATIC
		${CMAKE_CURRENT_SOURCE_DIR}/imconfig.h
		${CMAKE_CURRENT_SOURCE_DIR}/imgui.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/imgui.h
		${CMAKE_CURRENT_SOURCE_DIR}/imgui_demo.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/imgui_draw.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/imgui_internal.h
		${CMAKE_CURRENT_SOURCE_DIR}/imgui_widgets.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/imgui_tables.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/imstb_rectpack.h
		${CMAKE_CURRENT_SOURCE_DIR}/imstb_textedit.h
		${CMAKE_CURRENT_SOURCE_DIR}/imstb_truetype.h
)

set_target_properties(ImGUI
	PROPERTIES ${DEFAULT_PROJECT_OPTIONS}
)

target_include_directories(ImGUI
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}
	PUBLIC
		${DEFAULT_INCLUDE_DIRECTORIES}
	INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_link_libraries(ImGUI
	PRIVATE
	PUBLIC
		${DEFAULT_LIBRARIES}
)

target_compile_definitions(ImGUI
	PRIVATE
	PUBLIC
		${DEFAULT_COMPILE_DEFINITIONS}
)

target_compile_options(ImGUI
	PRIVATE
	PUBLIC
		${DEFAULT_COMPILE_OPTIONS}
)

target_link_libraries(ImGUI
	PRIVATE
	PUBLIC
		${DEFAULT_LINKER_OPTIONS}
)
