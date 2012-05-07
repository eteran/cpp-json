
#ifndef TOKEN_20110526_H_
#define TOKEN_20110526_H_

namespace json {
namespace detail {
	class token {
	public:
		enum type {
			type_invalid,
			type_delimeter,
			type_string,
			type_number,
			type_boolean,
			type_null
		};

	public:
		token() : type_(type_invalid) {
		}

		token(const std::string &s, type t) : string_(s), type_(t) {
		}

	public:
		bool operator==(const token &rhs) const { return string_ == rhs.string_ && type_ == rhs.type_; }
		bool operator!=(const token &rhs) const { return string_ != rhs.string_ || type_ != rhs.type_; }

	public:
		std::string string_;
		type        type_;

	};
}
}

#endif

