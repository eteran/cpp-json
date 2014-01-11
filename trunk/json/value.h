
#ifndef VALUE_20110526_H_
#define VALUE_20110526_H_

namespace json {

static const struct null_t {} null;

class array;
class object;

namespace detail {		
	template <class In>
	value get_value(In &it, const In &last);
}

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

	friend bool has_key(const value &v, const std::string &key);
	
	friend bool operator==(const value &lhs, const value &rhs);
	friend bool operator!=(const value &lhs, const value &rhs);
	
	template <class In>
	friend value detail::get_value(In &it, const In &last);
	
private:
	struct numeric_t {};
	// create a value from a numeric string, internal use only!
	value(const std::string &s, const numeric_t &); 

public:
	// intialize from basic types
	explicit value(const null_t &);
#if __cplusplus >= 201103L
	explicit value(const std::nullptr_t &);
#endif
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

private:
	boost::variant<boost::shared_ptr<object>, boost::shared_ptr<array>, std::string> value_;
	type                                                                             type_;
};

bool operator==(const value &lhs, const value &rhs);
bool operator!=(const value &lhs, const value &rhs);
	
}

#endif

