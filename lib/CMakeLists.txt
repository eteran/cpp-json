cmake_minimum_required(VERSION 3.0)

add_library(cpp-json INTERFACE
)

target_include_directories(cpp-json
	INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(cpp-json INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/include/cpp-json/array.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/cpp-json/array.tcc
	${CMAKE_CURRENT_SOURCE_DIR}/include/cpp-json/exception.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/cpp-json/json.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/cpp-json/json.tcc
	${CMAKE_CURRENT_SOURCE_DIR}/include/cpp-json/object.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/cpp-json/object.tcc
	${CMAKE_CURRENT_SOURCE_DIR}/include/cpp-json/parser.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/cpp-json/parser.tcc
	${CMAKE_CURRENT_SOURCE_DIR}/include/cpp-json/value.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/cpp-json/value.tcc

)

