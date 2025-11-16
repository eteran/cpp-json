
#ifndef CPP_JSON_ERROR_H_
#define CPP_JSON_ERROR_H_

#ifdef CPP_JSON_EXCEPTIONS_SUPPORTED
#define CPP_JSON_THROW(x) throw x
#elif defined(CPP_JSON_LIB_UNREACHABLE_SUPPORTED)
#include <utility>
#include <cassert>
#define CPP_JSON_THROW(...) assert(false); std::unreachable()
#else
#include <cstdlib>
#include <cassert>
#define CPP_JSON_THROW(...) assert(false); std::abort()
#endif

#include <cstddef>

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
	explicit lexing_error(std::size_t index)
		: index_(index) {}

	std::size_t index() const {
		return index_;
	}

private:
	std::size_t index_ = 0;
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
