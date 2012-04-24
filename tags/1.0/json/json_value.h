
#ifndef JSON_VALUE_20110526_H_
#define JSON_VALUE_20110526_H_

namespace json {

	class json_array;
	class json_object;

	class json_value {
		friend bool is_string(const json_value &v);
		friend bool is_bool(const json_value &v);
		friend bool is_number(const json_value &v);
		friend bool is_object(const json_value &v);
		friend bool is_array(const json_value &v);
		friend bool is_null(const json_value &v);
		
		friend std::string to_string(const json_value &v);
		friend bool to_bool(const json_value &v);
		friend double to_number(const json_value &v);
		friend json_object to_object(const json_value &v);
		friend json_array to_array(const json_value &v);
		
		friend size_t size(const json_value &v);
		friend set_type keys(const json_value &v);
		friend bool has_key(const json_value &v, const std::string &key);
		
	public:
		explicit json_value(double x) : value_(boost::lexical_cast<std::string>(x)), type_(number) {
		}
		
		explicit json_value(float x) : value_(boost::lexical_cast<std::string>(x)), type_(number) {
		}
		
		explicit json_value(long x) : value_(boost::lexical_cast<std::string>(x)), type_(number) {
		}
		
		explicit json_value(int x) : value_(boost::lexical_cast<std::string>(x)), type_(number) {
		}
		
		explicit json_value(const std::string &s) : value_(s), type_(string) {
		}
		
		explicit json_value(const char *s) : value_(s), type_(string) {
		}
		
		explicit json_value(bool b) : value_(b ? "true" : "false"), type_(boolean) {
		}
			
		explicit json_value(const json_array &a);
		explicit json_value(const json_object &o);
	
	public:
		json_value() : value_("null"), type_(null) {
		}

		explicit json_value(const boost::shared_ptr<json_object> &o) : value_(o), type_(object) {
		}

		explicit json_value(const boost::shared_ptr<json_array> &a) : value_(a), type_(array) {
		}

		explicit json_value(const json_token &token) : value_(token.string_) {
		
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

	public:
		json_value(const json_value &other) : value_(other.value_), type_(other.type_) {
		}
		
		json_value &operator=(const json_value &rhs) {
			json_value(rhs).swap(*this);
			return *this;
		}
		
		void swap(json_value &other) {
			using std::swap;
			swap(value_, other.value_);
		}
		
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
		const json_value operator[](const std::string &key) const;
		const json_value operator[](std::size_t n) const;

	private:
		boost::variant<boost::shared_ptr<json_object>, boost::shared_ptr<json_array>, std::string> value_;
		type                                                                                       type_;
	};
}

#endif

