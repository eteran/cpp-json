
#ifndef JSON_ERROR_20120104_H_
#define JSON_ERROR_20120104_H_

#include <exception>

class json_exception            : public std::exception {};
class boolean_expected          : public json_exception {};
class brace_expected            : public json_exception {};
class bracket_expected          : public json_exception {};
class colon_expected            : public json_exception {};
class invalid_string            : public json_exception {};
class invalid_type_cast         : public json_exception {};
class invalid_unicode_character : public json_exception {};
class keyword_expected          : public json_exception {};
class string_expected           : public json_exception {};
class value_expected            : public json_exception {};

#endif
