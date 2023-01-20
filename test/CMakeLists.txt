cmake_minimum_required(VERSION 3.15)

add_executable(example1 example1.cpp)
add_executable(example2 example2.cpp)
add_executable(example3 example3.cpp)

target_link_libraries(example1 PUBLIC cpp-json)
target_link_libraries(example2 PUBLIC cpp-json)
target_link_libraries(example3 PUBLIC cpp-json)

set_property(TARGET example1 PROPERTY CXX_STANDARD 17)
set_property(TARGET example2 PROPERTY CXX_STANDARD 17)
set_property(TARGET example3 PROPERTY CXX_STANDARD 17)

target_compile_options(example1 PUBLIC -pedantic -W -Wall -Wmissing-field-initializers -Wunused -Wshadow)
target_compile_options(example2 PUBLIC -pedantic -W -Wall -Wmissing-field-initializers -Wunused -Wshadow)
target_compile_options(example3 PUBLIC -pedantic -W -Wall -Wmissing-field-initializers -Wunused -Wshadow)

add_test(
    NAME example1
    COMMAND $<TARGET_FILE:example1>
)

add_test(
    NAME example2
    COMMAND $<TARGET_FILE:example2>
)

add_test(
    NAME example3
    COMMAND $<TARGET_FILE:example3>
)
