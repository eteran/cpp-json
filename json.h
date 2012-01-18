
#ifndef JSON_20110525_H_
#define JSON_20110525_H_

/* TODO: support unicode

           00 00 00 xx  UTF-32BE
           00 xx 00 xx  UTF-16BE
           xx 00 00 00  UTF-32LE
           xx 00 xx 00  UTF-16LE
           xx xx xx xx  UTF-8
*/

#include "json_token.h"
#include "json_object.h"
#include "json_array.h"
#include "json_error.h"
#include "json_value.h"

#include <string>
#include <boost/shared_ptr.hpp>

namespace json {

	typedef json_value value;	
		
	template <class In>
	value parse(In first, In last);
	
	value parse(std::istream &is);
	
	value parse(const std::string &s);
	
	inline bool is_string(const value &v) { return (v.type_ == json_value::string); }
	inline bool is_bool(const value &v)   { return (v.type_ == json_value::boolean); }
	inline bool is_number(const value &v) { return (v.type_ == json_value::number); }
	inline bool is_object(const value &v) { return (v.type_ == json_value::object); }
	inline bool is_array(const value &v)  { return (v.type_ == json_value::array); }
	inline bool is_null(const value &v)   { return (v.type_ == json_value::null); }
	
	std::string to_string(const value &v);
	bool to_bool(const value &v);
	double to_number(const value &v);
	
	size_t size(const value &v);
	boost::unordered_set<std::string> keys(const value &v);
	bool has_key(const value &v, const std::string &key);
}

#include "json.tcc"

#endif

