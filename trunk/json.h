
#ifndef JSON_20110525_H_
#define JSON_20110525_H_

/* TODO: support unicode
	00 00 00 xx  UTF-32BE
	00 xx 00 xx  UTF-16BE
	xx 00 00 00  UTF-32LE
	xx 00 xx 00  UTF-16LE
	xx xx xx xx  UTF-8
*/

#include "json/json_token.h"
#include "json/json_error.h"
#include "json/json_object.h"
#include "json/json_array.h"
#include "json/json_value.h"
#include <string>

namespace json {

	typedef json_value  value;
	typedef json_array  array;
	typedef json_object object;
		
	template <class In>
	value parse(In first, In last);
	
	value parse(std::istream &is);
	
	value parse(const std::string &s);
	value from_string(const std::string &s);
	
	inline bool is_string(const value &v);
	inline bool is_bool(const value &v);
	inline bool is_number(const value &v);
	inline bool is_object(const value &v);
	inline bool is_array(const value &v); 
	inline bool is_null(const value &v);
	
	std::string to_string(const value &v);
	bool        to_bool(const value &v);
	double      to_number(const value &v);
	
	size_t size(const value &v);
	boost::unordered_set<std::string> keys(const value &v);
	bool has_key(const value &v, const std::string &key);
	
	std::string pretty_print(const value &v);
	std::string pretty_print(const array &a);
	std::string pretty_print(const object &o);
}

#include "json/json.tcc"
#include "json/json_object.tcc"
#include "json/json_array.tcc"
#include "json/json_value.tcc"

#endif
