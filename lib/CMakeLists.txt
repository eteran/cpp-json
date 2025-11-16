cmake_minimum_required(VERSION 3.15)

file(GLOB_RECURSE library_headers "include/cpp-json/*.h")
add_library(cpp-json INTERFACE
    ${library_headers}
)
# Set up file tabs in Visual Studio
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/cpp-json/ PREFIX "Header Files" FILES ${library_headers})

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
