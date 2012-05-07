
#ifndef OBJECT_20120424_TCC_
#define OBJECT_20120424_TCC_

namespace json {

//------------------------------------------------------------------------------
// Name: object()
//------------------------------------------------------------------------------
inline object::object() {
}

//------------------------------------------------------------------------------
// Name: object(const object &other)
//------------------------------------------------------------------------------
inline object::object(const object &other) : values_(other.values_) {
}

//------------------------------------------------------------------------------
// Name: operator=(const object &rhs)
//------------------------------------------------------------------------------
inline object &object::operator=(const object &rhs) {
	object(rhs).swap(*this);
	return *this;
}

//------------------------------------------------------------------------------
// Name: operator[](const std::string &key) const
//------------------------------------------------------------------------------
inline const value object::operator[](const std::string &key) const {
	map_type::const_iterator it = values_.find(key);
	if(it != values_.end()) {
		return it->second;
	}

	throw invalid_index();
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
inline value &object::operator[](const std::string &key) {
	map_type::iterator it = values_.find(key);
	if(it != values_.end()) {
		return it->second;
	}

	throw invalid_index();
}

//------------------------------------------------------------------------------
// Name: append(const std::string &key, const T &v)
//------------------------------------------------------------------------------
template <class T>
inline object &object::append(const std::string &key, const T &v) {
	values_.insert(std::make_pair(key, value(v)));
	return *this;
}

//------------------------------------------------------------------------------
// Name: swap(object &other)
//------------------------------------------------------------------------------
inline void object::swap(object &other) {
	using std::swap;
	swap(values_, other.values_);
}

}

#endif
