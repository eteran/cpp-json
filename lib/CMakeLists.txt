cmake_minimum_required(VERSION 3.15)

add_library(cpp-json INTERFACE
)

target_include_directories(cpp-json
	INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(cpp-json INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/include/cpp-json/json.h
)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(cpp-json INTERFACE
        -Werror=reorder
    )
elseif(MSVC)
    target_compile_options(cpp-json INTERFACE
        /we5038
    )
endif()
