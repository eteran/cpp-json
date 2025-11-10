cmake_minimum_required (VERSION 3.15)
project(cpp-json CXX)

enable_testing()

if (MSVC)
    add_compile_options(/EHsc)
    if (MSVC_VERSION GREATER_EQUAL 1914)
        add_compile_options(/Zc:__cplusplus /Zc:throwingNew /Zc:externConstexpr)
    endif()
endif()

add_subdirectory(lib)
add_subdirectory(test)
