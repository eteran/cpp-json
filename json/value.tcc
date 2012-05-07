
#ifndef VALUE_20120424_TCC_
#define VALUE_20120424_TCC_

namespace json {

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
inline value::value() : value_("null"), type_(type_null) {
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
inline value::value(const boost::shared_ptr<object> &o) : value_(o), type_(type_object) {
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
inline value::value(const boost::shared_ptr<array> &a) : value_(a), type_(type_array) {
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
inline value::value(const detail::token &token) : value_(token.string_) {

	switch(token.type_) {
	case detail::token::type_string:
		type_ = type_string;
		break;
	case detail::token::type_number:
		type_ = type_number;
		break;
	case detail::token::type_boolean:
		type_ = type_boolean;
		break;
	case detail::token::type_null:
		type_ = type_null;
		break;
	default:
		type_ = type_invalid;
	}
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
inline value::value(double x) : value_(boost::lexical_cast<std::string>(x)), type_(type_number) {
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
inline value::value(float x) : value_(boost::lexical_cast<std::string>(x)), type_(type_number) {
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
inline value::value(long x) : value_(boost::lexical_cast<std::string>(x)), type_(type_number) {
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
inline value::value(int x) : value_(boost::lexical_cast<std::string>(x)), type_(type_number) {
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
inline value::value(const std::string &s) : value_(s), type_(type_string) {
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
inline value::value(const char *s) : value_(s), type_(type_string) {
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
inline value::value(bool b) : value_(b ? "true" : "false"), type_(type_boolean) {
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
inline value::value(const value &other) : value_(other.value_), type_(other.type_) {
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
inline value &value::operator=(const value &rhs) {
	value(rhs).swap(*this);
	return *this;
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
inline void value::swap(value &other) {
	using std::swap;
	swap(value_, other.value_);
	swap(type_, other.type_);
}

//------------------------------------------------------------------------------
// Name: operator[](const std::string &key) const
//------------------------------------------------------------------------------
inline const value value::operator[](const std::string &key) const {
	if(!is_object(*this)) {
		throw invalid_type_cast();
	}
	
	boost::shared_ptr<object> p = boost::get<boost::shared_ptr<object> >(value_);
	return (*p)[key];
}

//------------------------------------------------------------------------------
// Name: operator[](std::size_t n) const
//------------------------------------------------------------------------------
inline const value value::operator[](std::size_t n) const {
	if(!is_array(*this)) {
		throw invalid_type_cast();
	}

	boost::shared_ptr<array> p = boost::get<boost::shared_ptr<array> >(value_);
	return (*p)[n];
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
inline value &value::operator[](const std::string &key) {
	if(!is_object(*this)) {
		throw invalid_type_cast();
	}
	
	boost::shared_ptr<object> p = boost::get<boost::shared_ptr<object> >(value_);
	return (*p)[key];
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
inline value &value::operator[](std::size_t n) {
	if(!is_array(*this)) {
		throw invalid_type_cast();
	}

	boost::shared_ptr<array> p = boost::get<boost::shared_ptr<array> >(value_);
	return (*p)[n];
}

//------------------------------------------------------------------------------
// Name: value(const array &a)
//------------------------------------------------------------------------------
inline value::value(const array &a) : value_(boost::make_shared<array>(a)), type_(type_array) {
}

//------------------------------------------------------------------------------
// Name: value(const object &o)
//------------------------------------------------------------------------------
inline value::value(const object &o) : value_(boost::make_shared<object>(o)), type_(type_object) {
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
inline bool value::operator==(const value &rhs) const {
	if(type_ == rhs.type_) {
		switch(type_) {
		case type_string:
			return to_string(*this) == to_string(rhs);
		case type_number:
			return to_number(*this) == to_number(rhs);
		case type_null:
			return true;
		case type_boolean:
			return to_bool(*this) == to_bool(rhs);
		case type_array:
			assert(0);
		case type_object:
			assert(0);
		case type_invalid:
			break;
		}
	}
	return false;
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
inline bool value::operator!=(const value &rhs) const {
	return !(*this == rhs);
}

}

#endif
