set(CHIPMUNK_VERSION_MAJOR 7)
set(CHIPMUNK_VERSION_MINOR 0)
set(CHIPMUNK_VERSION_PATCH 3)
set(CHIPMUNK_VERSION "${CHIPMUNK_VERSION_MAJOR}.${CHIPMUNK_VERSION_MINOR}.${CHIPMUNK_VERSION_PATCH}")
message("Configuring Chipmunk2D version ${CHIPMUNK_VERSION}")

file(GLOB chipmunk_source_files "${CMAKE_CURRENT_SOURCE_DIR}/source/*.c")
file(GLOB chipmunk_public_header "${CMAKE_CURRENT_SOURCE_DIR}/include/chipmunk/*.h")

add_library(Chipmunk STATIC
		${chipmunk_source_files}
		${chipmunk_public_header}
)

set_target_properties(Chipmunk
	PROPERTIES ${DEFAULT_PROJECT_OPTIONS}
)

target_include_directories(Chipmunk
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/include
	PUBLIC
		${DEFAULT_INCLUDE_DIRECTORIES}
	INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
