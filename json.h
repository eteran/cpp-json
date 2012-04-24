
#ifndef JSON_20110525_H_
#define JSON_20110525_H_

#define USE_BOOST_UNORDERED

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

#ifdef USE_BOOST_UNORDERED
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#else
#include <map>
#include <set>

#endif

#include <algorithm>
#include <exception>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <cstdio>

namespace json {
	class json_value;
	class json_array;
	class json_object;

#ifdef USE_BOOST_UNORDERED
	typedef boost::unordered_map<std::string, json_value> map_type;
	typedef boost::unordered_set<std::string>             set_type;
#else
	typedef std::map<std::string, json_value> map_type;
	typedef std::set<std::string>             set_type;
#endif
	
	inline bool is_string(const json_value &v);
	inline bool is_bool(const json_value &v);
	inline bool is_number(const json_value &v);
	inline bool is_object(const json_value &v);
	inline bool is_array(const json_value &v); 
	inline bool is_null(const json_value &v);
	
	std::string to_string(const json_value &v);
	bool        to_bool(const json_value &v);
	double      to_number(const json_value &v);
	
	size_t size(const json_value &v);
	set_type keys(const json_value &v);
	bool has_key(const json_value &v, const std::string &key);
	
	std::string pretty_print(const json_value &v);
	std::string pretty_print(const json_array &a);
	std::string pretty_print(const json_object &o);
	
	template <class In>
	json_value parse(In first, In last);
	
	json_value parse(std::istream &is);
	
	json_value parse(const std::string &s);
	json_value from_string(const std::string &s);
}

#include "json/json_token.h"
#include "json/json_error.h"
#include "json/json_object.h"
#include "json/json_array.h"
#include "json/json_value.h"

namespace json {

	typedef json_value  value;
	typedef json_array  array;
	typedef json_object object;
}

#include "json/json.tcc"
#include "json/json_object.tcc"
#include "json/json_array.tcc"
#include "json/json_value.tcc"

#endif

