
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
	return at(key);
}

//------------------------------------------------------------------------------
// Name: operator[]
//------------------------------------------------------------------------------
inline value &object::operator[](const std::string &key) {
	return at(key);
}

//------------------------------------------------------------------------------
// Name: at
//------------------------------------------------------------------------------
inline const value object::at(const std::string &key) const {
	map_type::const_iterator it = values_.find(key);
	if(it != values_.end()) {
		return it->second;
	}

	throw invalid_index();
}

//------------------------------------------------------------------------------
// Name: at
//------------------------------------------------------------------------------
inline value &object::at(const std::string &key) {
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
inline object &object::insert(const std::pair<std::string, T> &p) {
	values_.insert(p);
	return *this;
}

//------------------------------------------------------------------------------
// Name: insert
//------------------------------------------------------------------------------
template <class T>
inline object &object::insert(const std::string &key, const T &v) {
	return insert(std::make_pair(key, value(v)));
}

//------------------------------------------------------------------------------
// Name: swap
//------------------------------------------------------------------------------
inline void object::swap(object &other) {
	using std::swap;
	swap(values_, other.values_);
}

//------------------------------------------------------------------------------
// Name: operator==
//------------------------------------------------------------------------------
inline bool operator==(const object &lhs, const object &rhs) {
	if(lhs.values_.size() == rhs.values_.size()) {
		return lhs.values_ == rhs.values_;
	}
	return false;
}

//------------------------------------------------------------------------------
// Name: operator!=
//------------------------------------------------------------------------------
inline bool operator!=(const object &lhs, const object &rhs) {
	return !(lhs == rhs);
}

}

#endif
