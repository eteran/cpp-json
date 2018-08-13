cmake_minimum_required (VERSION 3.0)

add_executable(example1 example1.cpp)
add_executable(example2 example2.cpp)
add_executable(example3 example3.cpp)
add_executable(example4 example4.cpp)

target_link_libraries(example1 LINK_PUBLIC cpp-json)
target_link_libraries(example2 LINK_PUBLIC cpp-json)
target_link_libraries(example3 LINK_PUBLIC cpp-json)
target_link_libraries(example4 LINK_PUBLIC cpp-json)

set_property(TARGET example1 PROPERTY CXX_STANDARD 11)
set_property(TARGET example2 PROPERTY CXX_STANDARD 11)
set_property(TARGET example3 PROPERTY CXX_STANDARD 11)
set_property(TARGET example4 PROPERTY CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -W -Wall -Wmissing-field-initializers -Wunused -Wshadow")
