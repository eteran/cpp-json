
#ifndef VALUE_20120424_TCC_
#define VALUE_20120424_TCC_

namespace json {

//------------------------------------------------------------------------------
// Name: ~value
//------------------------------------------------------------------------------
inline value::~value() {
	using std::string;

	switch(type_) {
	case value::type_string:
	case value::type_number:
	case value::type_null:
	case value::type_boolean:
		reinterpret_cast<std::string *>(&value_)->~string();
		break;
	case value::type_array:
		reinterpret_cast<array_pointer *>(&value_)->~array_pointer();
		break;
	case value::type_object:
		reinterpret_cast<object_pointer *>(&value_)->~object_pointer();
		break;
	case value::type_invalid:
		break;
	}
}

//------------------------------------------------------------------------------
// Name: value
//------------------------------------------------------------------------------
inline value::value() : type_(type_invalid) {

}

//------------------------------------------------------------------------------
// Name: value
//------------------------------------------------------------------------------
inline value::value(const std::nullptr_t &): type_(type_null) {
	new (&value_) std::string("null");
}

//------------------------------------------------------------------------------
// Name: value
//------------------------------------------------------------------------------
inline value::value(object_pointer o) : type_(type_object) {
	new (&value_) object_pointer(std::move(o));
}

//------------------------------------------------------------------------------
// Name: value
//------------------------------------------------------------------------------
inline value::value(array_pointer a) : type_(type_array) {
	new (&value_) array_pointer(std::move(a));
}

//------------------------------------------------------------------------------
// Name: value
//------------------------------------------------------------------------------
inline value::value(std::string s, const numeric_t &) : type_(type_number) {
	new (&value_) std::string(std::move(s));
}

//------------------------------------------------------------------------------
// Name: value
//------------------------------------------------------------------------------
inline value::value(std::string s) : type_(type_string) {
	new (&value_) std::string(std::move(s));
}

//------------------------------------------------------------------------------
// Name: value
//------------------------------------------------------------------------------
template <class T, typename>
value::value(T n) : type_(type_number) {
    new (&value_) std::string(std::to_string(n));
}

//------------------------------------------------------------------------------
// Name: value
//------------------------------------------------------------------------------
inline value::value(const char *s) : type_(type_string) {
	new (&value_) std::string(s);
}

//------------------------------------------------------------------------------
// Name: value
//------------------------------------------------------------------------------
inline value::value(bool b) : type_(type_boolean) {
	new (&value_) std::string(b ? "true" : "false");
}

//------------------------------------------------------------------------------
// Name: value
//------------------------------------------------------------------------------
inline value::value(const value &other) : type_(other.type_) {

	// copy from the other object
	switch(type_) {
	case value::type_string:
	case value::type_number:
	case value::type_null:
	case value::type_boolean:
		new (&value_) std::string(*reinterpret_cast<const std::string *>(&other.value_));
		break;
	case value::type_array:
		new (&value_) array_pointer(*reinterpret_cast<const array_pointer *>(&other.value_));
		break;
	case value::type_object:
		new (&value_) object_pointer(*reinterpret_cast<const object_pointer *>(&other.value_));
		break;
	case value::type_invalid:
		break;
	}
}

//------------------------------------------------------------------------------
// Name: operator=
//------------------------------------------------------------------------------
inline value &value::operator=(const value &rhs) {
	if(this != &rhs) {
		value(rhs).swap(*this);
	}
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
	return as_object()[key];
}

//------------------------------------------------------------------------------
// Name: operator[]
//------------------------------------------------------------------------------
inline const value value::operator[](std::size_t n) const {
	return as_array()[n];
}

//------------------------------------------------------------------------------
// Name: operator[]
//------------------------------------------------------------------------------
inline value &value::operator[](const std::string &key) {
	return as_object()[key];
}

//------------------------------------------------------------------------------
// Name: operator[]
//------------------------------------------------------------------------------
inline value &value::operator[](std::size_t n) {
	return as_array()[n];
}

//------------------------------------------------------------------------------
// Name: value
//------------------------------------------------------------------------------
inline value::value(const array &a) : type_(type_array) {
	new (&value_) array_pointer(std::make_shared<array>(a));
}

//------------------------------------------------------------------------------
// Name: value
//------------------------------------------------------------------------------
inline value::value(const object &o) : type_(type_object) {
	new (&value_) object_pointer(std::make_shared<object>(o));
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

//------------------------------------------------------------------------------
// Name: as_string
//------------------------------------------------------------------------------
inline const std::string &value::as_string() const {

	switch(type_) {
	case value::type_string:
	case value::type_number:
	case value::type_null:
	case value::type_boolean:	
		return *reinterpret_cast<const std::string *>(&value_);
	default:
		throw invalid_type_cast();
	}
}

//------------------------------------------------------------------------------
// Name: as_string
//------------------------------------------------------------------------------
inline std::string &value::as_string() {
	switch(type_) {
	case value::type_string:
	case value::type_number:
	case value::type_null:
	case value::type_boolean:	
		return *reinterpret_cast<std::string *>(&value_);
	default:
		throw invalid_type_cast();
	}
}

//------------------------------------------------------------------------------
// Name: as_object
//------------------------------------------------------------------------------
inline const object &value::as_object() const {
	if(type_ != type_object) {
		throw invalid_type_cast();
	}
	return **reinterpret_cast<const object_pointer *>(&value_);
}

//------------------------------------------------------------------------------
// Name: as_object
//------------------------------------------------------------------------------
inline object &value::as_object() {
	if(type_ != type_object) {
		throw invalid_type_cast();
	}
	return **reinterpret_cast<object_pointer *>(&value_);
}

//------------------------------------------------------------------------------
// Name: as_array
//------------------------------------------------------------------------------
inline const array &value::as_array() const {
	if(type_ != type_array) {
		throw invalid_type_cast();
	}
	return **reinterpret_cast<const array_pointer *>(&value_);
}

//------------------------------------------------------------------------------
// Name: as_array
//------------------------------------------------------------------------------
inline array &value::as_array() {
	if(type_ != type_array) {
		throw invalid_type_cast();
	}
	return **reinterpret_cast<array_pointer *>(&value_);
}

}

#endif
