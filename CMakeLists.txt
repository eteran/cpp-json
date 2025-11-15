cmake_minimum_required (VERSION 3.15)
project(cpp-json CXX)

if (NOT "${CMAKE_CXX_STANDARD}")
    set(CMAKE_CXX_STANDARD 17)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

if (MSVC)
    add_compile_options(/EHsc /Zc:throwingNew)
    if (MSVC_VERSION GREATER_EQUAL 1913)
        add_compile_options(/Zc:externConstexpr)
    endif()
    if (MSVC_VERSION GREATER_EQUAL 1914)
        add_compile_options(/Zc:__cplusplus)
    endif()
endif()

add_subdirectory(lib)
add_subdirectory(test)
