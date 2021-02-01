cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

function(set_policy POL VAL)
	if(POLICY ${POL})
		cmake_policy(SET ${POL} ${VAL})
	endif()
endfunction(set_policy)

set_policy(CMP0028 NEW) # ENABLE CMP0028: Double colon in target name means ALIAS or IMPORTED target.
set_policy(CMP0054 NEW) # ENABLE CMP0054: Only interpret if() arguments as variables or keywords when unquoted.
set_policy(CMP0042 NEW) # ENABLE CMP0042: MACOSX_RPATH is enabled by default.
set_policy(CMP0063 NEW) # ENABLE CMP0063: Honor visibility properties for all target types.
set_policy(CMP0048 NEW) # ENABLE CMP0048: Follow project() VERSION setting
set_policy(CMP0072 NEW) # ENABLE CMP0072: FindOpenGL prefers GLVND by default when available.

list(APPEND CMAKE_MODULE_PATH
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake"
)
include(GenerateExportHeader)
include(CPM)

project(Callisto VERSION 0.1)
include(CompileOptions)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/out")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/out")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/out")

add_subdirectory(deps)
add_subdirectory(scripting)
add_subdirectory(game)
