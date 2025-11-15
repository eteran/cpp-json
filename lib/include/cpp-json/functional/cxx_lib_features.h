#pragma once

/*
 *  This header detects optional C++ Standard Library features.
 */

#if __has_include(<version>)
#include <version>
#endif

#if __cpp_lib_unreachable >= 202202L
#define CPP_JSON_LIB_UNREACHABLE_SUPPORTED
#endif
