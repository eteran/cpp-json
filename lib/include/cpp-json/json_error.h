
#ifndef JSON_ERROR_H_
#define JSON_ERROR_H_

#include <cstddef>

namespace json {

// general error
class exception {
public:
	size_t line   = static_cast<size_t>(0);
	size_t column = static_cast<size_t>(0);
};

// parsing errors
class brace_expected : public exception {};
class bracket_expected : public exception {};
class colon_expected : public exception {};
class string_expected : public exception {};
class value_expected : public exception {};

// lexing errors
class invalid_unicode_character : public exception {};
class utf16_surrogate_expected : public exception {};
class quote_expected : public exception {};
class invalid_number : public exception {};

// serialization errors
class invalid_utf8_string : public exception {};

// usage errors
class invalid_type_cast : public exception {};
class invalid_index : public exception {};

// pointer errors
class invalid_path : public exception {};
class invalid_pointer_syntax : public exception {};
}

#endif
