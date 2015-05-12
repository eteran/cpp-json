
#ifndef EXCEPTION_20120104_H_
#define EXCEPTION_20120104_H_

namespace json {

// general error
class exception                 : public std::exception {
public:
	exception() : location(-1) {
	}
public:
	int location;
};

// parsing errors
class boolean_expected          : public exception {};
class brace_expected            : public exception {};
class bracket_expected          : public exception {};
class colon_expected            : public exception {};
class hex_character_expected    : public exception {};
class quote_expected            : public exception {};
class invalid_unicode_character : public exception {};
class keyword_expected          : public exception {};
class string_expected           : public exception {};
class value_expected            : public exception {};
class utf16_surrogate_expected  : public exception {};
class invalid_number            : public exception {};
class invalid_utf8_string       : public exception {};

// usage errors
class invalid_type_cast         : public exception {};
class invalid_index             : public exception {};

}

#endif
