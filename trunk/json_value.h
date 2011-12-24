
#ifndef JSON_VALUE_20110526_H_
#define JSON_VALUE_20110526_H_

#include "json_token.h"

#include <boost/shared_ptr.hpp>

namespace json {

	class json_array;
	class json_object;

	class json_value {
	public:
		json_value() : type_(invalid) {
		}

		explicit json_value(const boost::shared_ptr<json_object> &o) : object_(o), type_(object) {
		}

		explicit json_value(const boost::shared_ptr<json_array> &a) : array_(a), type_(array) {
		}

		explicit json_value(const json_token &token) : token_(token) {
		
			switch(token.type_) {
			case json_token::string:
				type_ = string;
				break;
			case json_token::number:
				type_ = number;
				break;
			case json_token::boolean:
				type_ = boolean;
				break;
			case json_token::null:
				type_ = null;
				break;
			default:
				type_ = invalid;
			}
		}
		
	private:
		json_value(const json_value &);
		json_value &operator=(const json_value &);
		
	public:
		enum type {
			invalid,
			string,
			number,
			object,
			array,
			boolean,
			null
		};

	public:
		boost::shared_ptr<json_object> object_;
		boost::shared_ptr<json_array>  array_;
		json_token                     token_; // a string, number, bool or null
		type                           type_;
	};
}

#endif

