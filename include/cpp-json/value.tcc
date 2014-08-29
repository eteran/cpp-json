
#ifndef VALUE_20120424_TCC_
#define VALUE_20120424_TCC_

namespace json {

//------------------------------------------------------------------------------
// Name: value
//------------------------------------------------------------------------------
inline value::value(const null_t &) : value_("null"), type_(type_null) {
}

#if __cplusplus >= 201103L
//------------------------------------------------------------------------------
// Name: value
//------------------------------------------------------------------------------
inline value::value(const std::nullptr_t &): value_("null"), type_(type_null) {
}

//------------------------------------------------------------------------------
// Name: array
//------------------------------------------------------------------------------
inline value::value(value &&other) : value_(other.value_), type_(other.type_) {
	other.value_ = boost::blank();
	other.type_  = type_invalid;
}

//------------------------------------------------------------------------------
// Name: operator=
//------------------------------------------------------------------------------
inline value &value::operator=(value &&rhs) {
	if(this != &rhs) {
		value_     = rhs.value_;
		type_      = rhs.type_;
		rhs.value_ = boost::blank();
		rhs.type_  = type_invalid;
	}
	return *this;
}
#endif

//------------------------------------------------------------------------------
// Name: value
//------------------------------------------------------------------------------
inline value::value(const object_pointer &o) : value_(o), type_(type_object) {
}

//------------------------------------------------------------------------------
// Name: value
//------------------------------------------------------------------------------
inline value::value(const array_pointer &a) : value_(a), type_(type_array) {
}

//------------------------------------------------------------------------------
// Name: value
//------------------------------------------------------------------------------
inline value::value(const std::string &s, const numeric_t &) : value_(s), type_(type_number) {
}

//------------------------------------------------------------------------------
// Name: value
//------------------------------------------------------------------------------
inline value::value(double x) :
#if __cplusplus >= 201103L
	value_(std::to_string(x)),
#else
	value_(boost::lexical_cast<std::string>(x)),
#endif
	type_(type_number) {
}

//------------------------------------------------------------------------------
// Name: value
//------------------------------------------------------------------------------
inline value::value(float x) :
#if __cplusplus >= 201103L
	value_(std::to_string(x)),
#else
	value_(boost::lexical_cast<std::string>(x)),
#endif
	type_(type_number) {
}

//------------------------------------------------------------------------------
// Name: value
//------------------------------------------------------------------------------
inline value::value(long x) : 
#if __cplusplus >= 201103L
	value_(std::to_string(x)),
#else
	value_(boost::lexical_cast<std::string>(x)),
#endif
	type_(type_number) {
}

//------------------------------------------------------------------------------
// Name: value
//------------------------------------------------------------------------------
inline value::value(int x) : 
#if __cplusplus >= 201103L
	value_(std::to_string(x)),
#else
	value_(boost::lexical_cast<std::string>(x)),
#endif
	type_(type_number) {
}

//------------------------------------------------------------------------------
// Name: value
//------------------------------------------------------------------------------
inline value::value(const std::string &s) : value_(s), type_(type_string) {
}

#if __cplusplus >= 201103L
//------------------------------------------------------------------------------
// Name: value
//------------------------------------------------------------------------------
inline value::value(object_pointer &&o) : value_(std::move(o)), type_(type_object) {
}

//------------------------------------------------------------------------------
// Name: value
//------------------------------------------------------------------------------
inline value::value(array_pointer &&a) : value_(std::move(a)), type_(type_array) {
}

//------------------------------------------------------------------------------
// Name: value
//------------------------------------------------------------------------------
inline value::value(std::string &&s, const numeric_t &) : value_(std::move(s)), type_(type_number) {
}

//------------------------------------------------------------------------------
// Name: value
//------------------------------------------------------------------------------
inline value::value(std::string &&s) : value_(std::move(s)), type_(type_string) {
}
#endif

//------------------------------------------------------------------------------
// Name: value
//------------------------------------------------------------------------------
inline value::value(const char *s) : value_(s), type_(type_string) {
}

//------------------------------------------------------------------------------
// Name: value
//------------------------------------------------------------------------------
inline value::value(bool b) : value_(b ? "true" : "false"), type_(type_boolean) {
}

//------------------------------------------------------------------------------
// Name: value
//------------------------------------------------------------------------------
inline value::value(const value &other) : value_(other.value_), type_(other.type_) {
}

//------------------------------------------------------------------------------
// Name: operator=
//------------------------------------------------------------------------------
inline value &value::operator=(const value &rhs) {
	value(rhs).swap(*this);
	return *this;
}

//------------------------------------------------------------------------------
// Name: swap
//------------------------------------------------------------------------------
inline void value::swap(value &other) {
	using std::swap;
	swap(value_, other.value_);
	swap(type_, other.type_);
}

//------------------------------------------------------------------------------
// Name: operator[]
//------------------------------------------------------------------------------
inline const value value::operator[](const std::string &key) const {
	return as_object(*this)[key];
}

//------------------------------------------------------------------------------
// Name: operator[]
//------------------------------------------------------------------------------
inline const value value::operator[](std::size_t n) const {
	return as_array(*this)[n];
}

//------------------------------------------------------------------------------
// Name: operator[]
//------------------------------------------------------------------------------
inline value &value::operator[](const std::string &key) {
	return as_object(*this)[key];
}

//------------------------------------------------------------------------------
// Name: operator[]
//------------------------------------------------------------------------------
inline value &value::operator[](std::size_t n) {
	return as_array(*this)[n];
}

//------------------------------------------------------------------------------
// Name: value
//------------------------------------------------------------------------------
inline value::value(const array &a) : 
#if __cplusplus >= 201103L
	value_(std::make_shared<array>(a)), 
#else
	value_(boost::make_shared<array>(a)), 
#endif
	type_(type_array) {
}

//------------------------------------------------------------------------------
// Name: value
//------------------------------------------------------------------------------
inline value::value(const object &o) : 
#if __cplusplus >= 201103L
	value_(std::make_shared<object>(o)),
#else
	value_(boost::make_shared<object>(o)),
#endif
	type_(type_object) {
}

//------------------------------------------------------------------------------
// Name: operator==
//------------------------------------------------------------------------------
inline bool operator==(const value &lhs, const value &rhs) {
	if(lhs.type_ == rhs.type_) {
		switch(lhs.type_) {
		case value::type_string:
			return as_string(lhs) == as_string(rhs);
		case value::type_number:
			return to_number(lhs) == to_number(rhs);
		case value::type_null:
			return true;
		case value::type_boolean:
			return to_bool(lhs) == to_bool(rhs);
		case value::type_array:
			return as_array(lhs) == as_array(rhs);
		case value::type_object:
			return as_object(lhs) == as_object(rhs);
		case value::type_invalid:
			break;
		}
	}
	return false;
}

//------------------------------------------------------------------------------
// Name: operator!=
//------------------------------------------------------------------------------
inline bool operator!=(const value &lhs, const value &rhs) {
	return !(lhs == rhs);
}

}

#endif
