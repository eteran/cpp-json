cmake_minimum_required(VERSION 3.15)

add_executable(example1 example1.cpp)
add_executable(example2 example2.cpp)
add_executable(example3 example3.cpp)
add_executable(example4 example4.cpp)

target_link_libraries(example1 PUBLIC cpp-json)
target_link_libraries(example2 PUBLIC cpp-json)
target_link_libraries(example3 PUBLIC cpp-json)
target_link_libraries(example4 PUBLIC cpp-json)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(example1 PUBLIC -pedantic -W -Wall -Wmissing-field-initializers -Wunused -Wshadow)
    target_compile_options(example2 PUBLIC -pedantic -W -Wall -Wmissing-field-initializers -Wunused -Wshadow)
    target_compile_options(example3 PUBLIC -pedantic -W -Wall -Wmissing-field-initializers -Wunused -Wshadow)
    target_compile_options(example4 PUBLIC -pedantic -W -Wall -Wmissing-field-initializers -Wunused -Wshadow)
elseif(MSVC)
    # multi-processor compilation, warning-level 4
    target_compile_options(example1 PUBLIC /MP /W4)
    target_compile_options(example2 PUBLIC /MP /W4)
    target_compile_options(example3 PUBLIC /MP /W4)
    target_compile_options(example4 PUBLIC /MP /W4)
endif()

add_test(
    NAME example1
    COMMAND $<TARGET_FILE:example1>
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/test/
)

add_test(
    NAME example2
    COMMAND $<TARGET_FILE:example2>
)

add_test(
    NAME example3
    COMMAND $<TARGET_FILE:example3>
)

add_test(
    NAME example4
    COMMAND $<TARGET_FILE:example4>
)
