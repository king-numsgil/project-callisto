string(TOUPPER ${CMAKE_SYSTEM_NAME} SYSTEM_NAME_UPPER)

set(X64 OFF)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(X64 ON)
endif()

set(DEFAULT_PROJECT_OPTIONS
	DEBUG_POSTFIX             "-d"
    CXX_STANDARD              20
    LINKER_LANGUAGE           "CXX"
    POSITION_INDEPENDENT_CODE ON
    CXX_VISIBILITY_PRESET     "hidden"
    CXX_EXTENSIONS            Off
)

set(DEFAULT_INCLUDE_DIRECTORIES)

set(DEFAULT_LIBRARIES
	stdc++fs
)

set(DEFAULT_COMPILE_DEFINITIONS
    SYSTEM_${SYSTEM_NAME_UPPER}
)

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
    set(DEFAULT_COMPILE_DEFINITIONS ${DEFAULT_COMPILE_DEFINITIONS}
        _SCL_SECURE_NO_WARNINGS
        _CRT_SECURE_NO_WARNINGS
    )
endif ()

set(DEFAULT_COMPILE_OPTIONS)

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
    set(DEFAULT_COMPILE_OPTIONS ${DEFAULT_COMPILE_OPTIONS}
        /MP           # -> build with multiple processes
        /W4           # -> warning level 4
        # /WX         # -> treat warnings as errors

        /wd4251       # -> disable warning: 'identifier': class 'type' needs to have dll-interface to be used by clients of class 'type2'
        /wd4592       # -> disable warning: 'identifier': symbol will be dynamically initialized (implementation limitation)
        /wd4201     # -> disable warning: nonstandard extension used: nameless struct/union (caused by GLM)
        # /wd4127     # -> disable warning: conditional expression is constant (caused by Qt)

        #$<$<CONFIG:Debug>:
        #/RTCc         # -> value is assigned to a smaller data type and results in a data loss
        #>

        $<$<CONFIG:Release>:
            /Gw        # -> whole program global optimization
            /GS-       # -> buffer security check: no
            /GL        # -> whole program optimization: enable link-time code generation (disables Zi)
            /GF        # -> enable string pooling
        >
    )
endif ()

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(DEFAULT_COMPILE_OPTIONS ${DEFAULT_COMPILE_OPTIONS}
        -Og
        -Wall -Wextra -Wunused

        -Wreorder
        -Wignored-qualifiers
        -Wmissing-braces
        -Wreturn-type
        -Wswitch
        -Wuninitialized
        -Wmissing-field-initializers
        -Wno-switch-default -Wno-double-promotion -Wno-old-style-cast
	    -Wno-missing-braces -Wno-unknown-pragmas -Wno-unused-parameter
	    -Wsuggest-final-types

        $<$<CXX_COMPILER_ID:GNU>:
            -Wmaybe-uninitialized

            $<$<VERSION_GREATER:$<CXX_COMPILER_VERSION>,4.8>:
                -Wpedantic

                -Wreturn-local-addr

                -Wno-literal-suffix
                -Wno-misleading-indentation
            >
        >

        $<$<CXX_COMPILER_ID:Clang>:
            -Wpedantic
        >

        $<$<PLATFORM_ID:Darwin>:
            -pthread
        >

        $<$<VERSION_LESS:${CMAKE_VERSION},3.1>:
            -std=c++20
        >
    )
endif ()

set(DEFAULT_LINKER_OPTIONS)

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" OR "${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
    set(DEFAULT_LINKER_OPTIONS
        -pthread
    )
endif()
