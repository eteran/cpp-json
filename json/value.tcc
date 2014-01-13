
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
inline value::value(value &&other) {
	value_       = other.value_;
	type_        = other.type_;
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

#if __cplusplus >= 201103L
//------------------------------------------------------------------------------
// Name: value
//------------------------------------------------------------------------------
inline value::value(std::string &&s, const numeric_t &) : value_(std::move(s)), type_(type_number) {
}
#endif

//------------------------------------------------------------------------------
// Name: value
//------------------------------------------------------------------------------
inline value::value(double x) : value_(boost::lexical_cast<std::string>(x)), type_(type_number) {
}

//------------------------------------------------------------------------------
// Name: value
//------------------------------------------------------------------------------
inline value::value(float x) : value_(boost::lexical_cast<std::string>(x)), type_(type_number) {
}

//------------------------------------------------------------------------------
// Name: value
//------------------------------------------------------------------------------
inline value::value(long x) : value_(boost::lexical_cast<std::string>(x)), type_(type_number) {
}

//------------------------------------------------------------------------------
// Name: value
//------------------------------------------------------------------------------
inline value::value(int x) : value_(boost::lexical_cast<std::string>(x)), type_(type_number) {
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
	if(!is_object(*this)) {
		throw invalid_type_cast();
	}
	
	object_pointer p = boost::get<object_pointer >(value_);
	return (*p)[key];
}

//------------------------------------------------------------------------------
// Name: operator[]
//------------------------------------------------------------------------------
inline const value value::operator[](std::size_t n) const {
	if(!is_array(*this)) {
		throw invalid_type_cast();
	}

	array_pointer p = boost::get<array_pointer >(value_);
	return (*p)[n];
}

//------------------------------------------------------------------------------
// Name: operator[]
//------------------------------------------------------------------------------
inline value &value::operator[](const std::string &key) {
	if(!is_object(*this)) {
		throw invalid_type_cast();
	}
	
	object_pointer p = boost::get<object_pointer>(value_);
	return (*p)[key];
}

//------------------------------------------------------------------------------
// Name: operator[]
//------------------------------------------------------------------------------
inline value &value::operator[](std::size_t n) {
	if(!is_array(*this)) {
		throw invalid_type_cast();
	}

	array_pointer p = boost::get<array_pointer>(value_);
	return (*p)[n];
}

//------------------------------------------------------------------------------
// Name: value
//------------------------------------------------------------------------------
inline value::value(const array &a) : value_(boost::make_shared<array>(a)), type_(type_array) {
}

//------------------------------------------------------------------------------
// Name: value
//------------------------------------------------------------------------------
inline value::value(const object &o) : value_(boost::make_shared<object>(o)), type_(type_object) {
}

//------------------------------------------------------------------------------
// Name: operator==
//------------------------------------------------------------------------------
inline bool operator==(const value &lhs, const value &rhs) {
	if(lhs.type_ == rhs.type_) {
		switch(lhs.type_) {
		case value::type_string:
			return to_string(lhs) == to_string(rhs);
		case value::type_number:
			return to_number(lhs) == to_number(rhs);
		case value::type_null:
			return true;
		case value::type_boolean:
			return to_bool(lhs) == to_bool(rhs);
		case value::type_array:
			return to_array(lhs) == to_array(rhs);
		case value::type_object:
			return to_object(lhs) == to_object(rhs);
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
