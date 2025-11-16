#ifndef CPP_JSON_CXX_CHECK_PREREQUISITES_H_
#define CPP_JSON_CXX_CHECK_PREREQUISITES_H_

/*
 *  This header detects missing core C++ language features on which cpp-json depends, bailing out with a hard error.
 */

#if (!defined(__has_include)) ||                                                                                       \
	((__cpp_constexpr < 201603L) ||                                                                              \
	 (__cpp_if_constexpr < 201606L || __cpp_inline_variables < 201606L || __cpp_structured_bindings < 201606L))
#error A fully C++17-compliant compiler is required.
#endif

#endif
