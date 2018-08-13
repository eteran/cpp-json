cmake_minimum_required(VERSION 3.0)

add_library(cpp-json INTERFACE
)

target_include_directories(cpp-json
	INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

