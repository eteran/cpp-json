
#ifndef JSON_ERROR_H_
#define JSON_ERROR_H_

#if __has_include(<version>)
// __cpp_lib_* macros
#include <version>
#endif

#if __cpp_exceptions >= 199711L
#define JSON_THROW(x) throw x
#elif __cpp_lib_unreachable >= 202202L
#include <utility>
#include <cassert>
#define JSON_THROW(...) assert(false); std::unreachable()
#else
#include <cstdlib>
#include <cassert>
#define JSON_THROW(...) assert(false); std::abort()
#endif

#include <stddef.h>

namespace json {

// general error
struct exception {};

// parsing errors
struct brace_expected : exception {};
struct bracket_expected : exception {};
struct colon_expected : exception {};
struct string_expected : exception {};
struct value_expected : exception {};

// lexing errors
struct lexing_error : exception {
	lexing_error(size_t index)
		: index_(index) {}

	size_t index() const {
		return index_;
	}

private:
	size_t index_ = 0;
};
struct invalid_unicode_character : lexing_error {
	using lexing_error::lexing_error;
};
struct utf16_surrogate_expected : lexing_error {
	using lexing_error::lexing_error;
};
struct quote_expected : lexing_error {
	using lexing_error::lexing_error;
};
struct invalid_number : lexing_error {
	using lexing_error::lexing_error;
};

// serialization errors
struct invalid_utf8_string : exception {};

// usage errors
struct invalid_type_cast : exception {};
struct invalid_index : exception {};

// pointer errors
struct invalid_path : exception {};
struct invalid_pointer_syntax : exception {};
}

#endif
