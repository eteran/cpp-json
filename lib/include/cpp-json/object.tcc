
#ifndef OBJECT_20120424_TCC_
#define OBJECT_20120424_TCC_

namespace json {

//------------------------------------------------------------------------------
// Name: object
//------------------------------------------------------------------------------
inline object::object(std::initializer_list<std::pair<std::string, value>> list) {

	for(auto &x : list) {
		values_.emplace_back(x);
	}
}

//------------------------------------------------------------------------------
// Name: insert
//------------------------------------------------------------------------------
template <class T>
inline object &object::insert(std::pair<std::string, T> &&p) {
	values_.emplace_back(std::move(p));
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
	auto it = std::find_if(values_.begin(), values_.end(), [&key](const std::pair<std::string, value> &entry) {
		return entry.first == key;
	});
	if(it != values_.end()) {
		return it->second;
	}

	throw invalid_index();
}

//------------------------------------------------------------------------------
// Name: at
//------------------------------------------------------------------------------
inline value &object::at(const std::string &key) {
	auto it = std::find_if(values_.begin(), values_.end(), [&key](const std::pair<std::string, value> &entry) {
		return entry.first == key;
	});

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
	values_.emplace_back(p);
	return *this;
}

//------------------------------------------------------------------------------
// Name: insert
//------------------------------------------------------------------------------
template <class T>
inline object &object::insert(const std::string &key, const T &v) {
	values_.emplace_back(std::make_pair(key, value(v)));
	return *this;
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
