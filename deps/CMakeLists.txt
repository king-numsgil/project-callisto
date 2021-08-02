CPMAddPackage(
	NAME GLFW
	GITHUB_REPOSITORY glfw/glfw
	GIT_TAG 3.3.4
	VERSION 3.3.4
	OPTIONS
		"BUILD_SHARED_LIBS On"
		"GLFW_BUILD_EXAMPLES Off"
		"GLFW_BUILD_TESTS Off"
		"GLFW_BUILD_DOCS Off"
)

CPMAddPackage(
	NAME Json
	VERSION 3.9.1
	URL https://github.com/nlohmann/json/releases/download/v3.9.1/include.zip
	URL_HASH SHA256=6bea5877b1541d353bd77bdfbdb2696333ae5ed8f9e8cc22df657192218cad91
)

if(Json_ADDED)
	add_library(Json INTERFACE)
	target_include_directories(Json INTERFACE ${Json_SOURCE_DIR}/include)
endif()

CPMAddPackage(
	NAME EnTT
	VERSION 3.8.1
	GITHUB_REPOSITORY skypjack/entt
)

CPMAddPackage(
	NAME lua
	GITHUB_REPOSITORY lua/lua
	VERSION 5.3.6
	DOWNLOAD_ONLY YES
)

if (lua_ADDED)
	# lua has no CMake support, so we create our own target

	file(GLOB lua_sources ${lua_SOURCE_DIR}/*.c)
	list(REMOVE_ITEM lua_sources ${lua_SOURCE_DIR}/onelua.c)
	add_library(lua STATIC ${lua_sources})

	if("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
		target_compile_options(lua PUBLIC -fPIC)
	endif()

	target_include_directories(lua
		PUBLIC
			$<BUILD_INTERFACE:${lua_SOURCE_DIR}>
	)
endif()

CPMAddPackage(
	NAME sol2
	URL https://github.com/ThePhD/sol2/archive/v3.2.2.zip
	VERSION 3.2.2
	DOWNLOAD_ONLY YES
)

if (sol2_ADDED)
	add_library(sol2 INTERFACE)
	target_include_directories(sol2 INTERFACE ${sol2_SOURCE_DIR}/include)
	target_link_libraries(sol2 INTERFACE lua)
endif()

CPMAddPackage(
	NAME Corrade
	GITHUB_REPOSITORY mosra/corrade
	VERSION 2020.06
	OPTIONS
		"BUILD_DEPRECATED Off"
)

CPMAddPackage(
	NAME Magnum
	GITHUB_REPOSITORY mosra/magnum
	VERSION 2020.06
	OPTIONS
		"BUILD_DEPRECATED Off"
		"WITH_GL On"
		"WITH_AUDIO On"
		"WITH_TRADE On"
		"WITH_GLFWAPPLICATION On"
		"WITH_ANYIMAGEIMPORTER On"
		"WITH_ANYAUDIOIMPORTER On"
		"WITH_ANYSCENEIMPORTER On"
)

CPMAddPackage(
	NAME MagnumPlugins
	GITHUB_REPOSITORY mosra/magnum-plugins
	VERSION 2020.06
	OPTIONS
		"BUILD_DEPRECATED Off"
		"WITH_STBIMAGECONVERTER On"
		"WITH_STBIMAGEIMPORTER On"
		"WITH_STBTRUETYPEFONT On"
		"WITH_DRFLACAUDIOIMPORTER On"
		"WITH_TINYGLTFIMPORTER On"
)

add_subdirectory(ImGUI)
