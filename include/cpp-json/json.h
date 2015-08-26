
#ifndef JSON_20110525_H_
#define JSON_20110525_H_

/* TODO(eteran): support unicode
	00 00 00 xx  UTF-32BE
	00 xx 00 xx  UTF-16BE
	xx 00 00 00  UTF-32LE
	xx 00 xx 00  UTF-16LE
	xx xx xx xx  UTF-8
*/

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <exception>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace json {

class value;
class array;
class object;

typedef std::shared_ptr<object> object_pointer;
typedef std::shared_ptr<array>  array_pointer;

// type testing
inline bool is_string(const value &v);
inline bool is_bool(const value &v);
inline bool is_number(const value &v);
inline bool is_object(const value &v);
inline bool is_array(const value &v);
inline bool is_null(const value &v);

// conversion (you get a copy)
inline std::string to_string(const value &v);
inline bool        to_bool(const value &v);
inline double      to_number(const value &v);
inline object      to_object(const value &v);
inline array       to_array(const value &v);

// interpretation (you get a reference)
inline object            &as_object(value &v);
inline array             &as_array(value &v);
inline std::string       &as_string(value &v);
inline const object      &as_object(const value &v);
inline const array       &as_array(const value &v);
inline const std::string &as_string(const value &v);

// does the given object have a given key?
inline bool has_key(const value &v, const std::string &key);
inline bool has_key(const object &o, const std::string &key);

// create a value from some JSON
template <class In>
inline value parse(In first, In last);
inline value parse(std::istream &is);
inline value parse(std::wistream &is);
inline value parse(std::istream &&is);
inline value parse(std::wistream &&is);
inline value parse(const std::string &s);
inline value parse(const std::wstring &s);

// convert a value to a JSON string
enum {
	ESCAPE_UNICODE = 0x01,
	PRETTY_PRINT   = 0x02
};

inline std::string stringify(const value &v, unsigned options);
inline std::string stringify(const array &a, unsigned options);
inline std::string stringify(const object &o, unsigned options);

inline std::string stringify(const value &v);
inline std::string stringify(const array &a);
inline std::string stringify(const object &o);

}

#include "exception.h"

#include "value.h"
#include "object.h"
#include "array.h"
#include "parser.h"

#include "json.tcc"
#include "object.tcc"
#include "array.tcc"
#include "value.tcc"
#include "parser.tcc"

#endif

