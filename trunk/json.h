
#ifndef JSON_20110525_H_
#define JSON_20110525_H_

/* TODO: support unicode
	00 00 00 xx  UTF-32BE
	00 xx 00 xx  UTF-16BE
	xx 00 00 00  UTF-32LE
	xx 00 xx 00  UTF-16LE
	xx xx xx xx  UTF-8
*/

#include <boost/cstdint.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/variant.hpp>
#include <boost/variant/get.hpp>
#include <boost/unordered_map.hpp>

#include <algorithm>
#include <exception>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

namespace json {

class value;
class array;
class object;

typedef boost::unordered_map<std::string, value> map_type;

// type testing
inline bool is_string(const value &v);
inline bool is_bool(const value &v);
inline bool is_number(const value &v);
inline bool is_object(const value &v);
inline bool is_array(const value &v); 
inline bool is_null(const value &v);

// conversion
std::string to_string(const value &v);
bool        to_bool(const value &v);
double      to_number(const value &v);
object      to_object(const value &v);
array       to_array(const value &v);

// does the given object have a given key?
bool has_key(const value &v, const std::string &key);
bool has_key(const object &o, const std::string &key);

// create a value from some JSON
template <class In>
value parse(In first, In last);
value parse(std::istream &is);
value parse(std::wistream &is);
value parse(const std::string &s);
value parse(const std::wstring &s);

// conert a value to a JSON string
enum {
	ESCAPE_UNICODE = 0x01,
	PRETTY_PRINT   = 0x02
};

std::string print(const value &v, unsigned options);
std::string print(const array &a, unsigned options);
std::string print(const object &o, unsigned options);

std::string print(const value &v);
std::string print(const array &a);
std::string print(const object &o);

}

#include "json/token.h"
#include "json/exception.h"

#include "json/object.h"
#include "json/array.h"
#include "json/value.h"

#include "json/json.tcc"
#include "json/object.tcc"
#include "json/array.tcc"
#include "json/value.tcc"

#endif
