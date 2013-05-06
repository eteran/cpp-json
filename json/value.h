
#ifndef VALUE_20110526_H_
#define VALUE_20110526_H_

namespace json {

	struct null {};

	class array;
	class object;

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
		friend object to_object(const value &v);
		friend array to_array(const value &v);
		
		friend size_t size(const value &v);
		friend set_type keys(const value &v);
		friend bool has_key(const value &v, const std::string &key);
		
	public:
		// intialize from basic types
		explicit value(const null &);
		explicit value(bool b);
		explicit value(const array &a);
		explicit value(const char *s);
		explicit value(const object &o);
		explicit value(const std::string &s);
		explicit value(double x);
		explicit value(float x);
		explicit value(int x);
		explicit value(long x);

	public:
		explicit value(const boost::shared_ptr<object> &o);
		explicit value(const boost::shared_ptr<array> &a);
		explicit value(const detail::token &token);

	public:
		value(const value &other);
		value &operator=(const value &rhs);
		
	public:
		void swap(value &other);
		
	public:
		enum type {
			type_invalid,
			type_string,
			type_number,
			type_object,
			type_array,
			type_boolean,
			type_null
		};
		
	public:
		const value operator[](const std::string &key) const;
		const value operator[](std::size_t n) const;
		value &operator[](const std::string &key);
		value &operator[](std::size_t n);
		
	public:
		bool operator==(const value &rhs) const;
		bool operator!=(const value &rhs) const;

	private:
		boost::variant<boost::shared_ptr<object>, boost::shared_ptr<array>, std::string> value_;
		type                                                                             type_;
	};
}

#endif

