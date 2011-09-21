
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
#include "json_value.h"
#include "json_object.h"
#include "json_array.h"

#include <string>
#include <exception>

#include <boost/shared_ptr.hpp>

class invalid_string    : public std::exception {};
class string_expected   : public std::exception {};
class value_expected    : public std::exception {};
class keyword_expected  : public std::exception {};
class boolean_expected  : public std::exception {};
class bracket_expected  : public std::exception {};
class brace_expected    : public std::exception {};
class colon_expected    : public std::exception {};
class invalid_type_cast : public std::exception {};

namespace json {

	class value {
		friend bool is_string(const value &v);
		friend bool is_bool(const value &v);
		friend bool is_number(const value &v);
		friend bool is_object(const value &v);
		friend bool is_array(const value &v);
		friend bool is_null(const value &v);
		
		friend std::string to_string(const value &v);
		friend bool to_bool(const value &v);
		friend double to_number(const value &v);
		
		friend size_t size(const value &v);
		
	public:
		value();
		value(const boost::shared_ptr<json_value> &v);
		
	public:
		value operator[](const std::string &key) const;
		value operator[](std::size_t n) const;

	private:
		boost::shared_ptr<json_value> value_;
	};
		
	template <class In>
	value parse(In first, const In &last);
	
	bool is_string(const value &v);
	bool is_bool(const value &v);
	bool is_number(const value &v);
	bool is_object(const value &v);
	bool is_array(const value &v);
	bool is_null(const value &v);
	
	std::string to_string(const value &v);
	bool to_bool(const value &v);
	double to_number(const value &v);
	
	size_t size(const value &v);
}

#include "json.tcc"


#endif

