cmake_minimum_required (VERSION 3.0)

project(cpp-json CXX)

set(json_SOURCES
    include/cpp-json/object.h
    include/cpp-json/array.tcc
    include/cpp-json/json.h
    include/cpp-json/exception.h
    include/cpp-json/value.h
    include/cpp-json/parser.h
    include/cpp-json/value.tcc
    include/cpp-json/array.h
    include/cpp-json/json.tcc
    include/cpp-json/object.tcc
    include/cpp-json/parser.tcc
)

include_directories(include)

add_executable(example1 example1.cc ${json_SOURCES})
add_executable(example2 example2.cc ${json_SOURCES})
add_executable(example3 example3.cc ${json_SOURCES})
add_executable(example4 example4.cc ${json_SOURCES})

set_property(TARGET example1 PROPERTY CXX_STANDARD 11)
set_property(TARGET example2 PROPERTY CXX_STANDARD 11)
set_property(TARGET example3 PROPERTY CXX_STANDARD 11)
set_property(TARGET example4 PROPERTY CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -W -Wall -Wmissing-field-initializers -Wunused -Wshadow")
