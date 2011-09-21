
#ifndef JSON_TOKEN_20110526_H_
#define JSON_TOKEN_20110526_H_

#include <string>

namespace json {
	class json_token {
	public:
		enum type {
			invalid,
			delimeter,
			string,
			number,
			boolean,
			null
		};

	public:
		json_token() : type_(invalid) {
		}

		json_token(const std::string &s, type t) : string_(s), type_(t) {
		}

	public:
		bool operator==(const json_token &rhs) const { return string_ == rhs.string_ && type_ == rhs.type_; }
		bool operator!=(const json_token &rhs) const { return string_ != rhs.string_ || type_ != rhs.type_; }

	public:
		std::string string_;
		type        type_;

	};
}

#endif

