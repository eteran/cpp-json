
#ifndef OBJECT_20120424_TCC_
#define OBJECT_20120424_TCC_

namespace json {

//------------------------------------------------------------------------------
// Name: object
//------------------------------------------------------------------------------
inline object::object() {
}

//------------------------------------------------------------------------------
// Name: object
//------------------------------------------------------------------------------
inline object::object(const object &other) : values_(other.values_) {
}

//------------------------------------------------------------------------------
// Name: operator=
//------------------------------------------------------------------------------
inline object &object::operator=(const object &rhs) {
	object(rhs).swap(*this);
	return *this;
}

//------------------------------------------------------------------------------
// Name: operator[]
//------------------------------------------------------------------------------
inline const value object::operator[](const std::string &key) const {
	map_type::const_iterator it = values_.find(key);
	if(it != values_.end()) {
		return it->second;
	}

	throw invalid_index();
}

//------------------------------------------------------------------------------
// Name: operator[]
//------------------------------------------------------------------------------
inline value &object::operator[](const std::string &key) {
	map_type::iterator it = values_.find(key);
	if(it != values_.end()) {
		return it->second;
	}

	throw invalid_index();
}

//------------------------------------------------------------------------------
// Name: insert
//------------------------------------------------------------------------------
template <class T>
inline object &object::insert(const std::string &key, const T &v) {
	values_.insert(std::make_pair(key, value(v)));
	return *this;
}

//------------------------------------------------------------------------------
// Name: swap
//------------------------------------------------------------------------------
inline void object::swap(object &other) {
	using std::swap;
	swap(values_, other.values_);
}

}

#endif
