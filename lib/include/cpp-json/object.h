
#ifndef OBJECT_20110526_H_
#define OBJECT_20110526_H_

namespace json {

class object;
class value;

class object {
	friend bool operator==(const object &lhs, const object &rhs);
	friend bool operator!=(const object &lhs, const object &rhs);

	template <class In>
	friend class parser;

private:
	using C = std::vector<std::pair<std::string, value>>;


public:
	using allocator_type         = typename C::allocator_type;
	using reference              = typename C::reference;
	using const_reference        = typename C::const_reference;
	using pointer                = typename C::pointer;
	using const_pointer          = typename C::const_pointer;
	using iterator               = typename C::iterator;
	using const_iterator         = typename C::const_iterator;
	using difference_type        = typename C::difference_type;
	using size_type              = typename C::size_type;

public:
	object()                             = default;
	object(const object &other)          = default;
	object(object &&other)               = default;
	object &operator=(const object &rhs) = default;
	object &operator=(object &&rhs)      = default;
	object(std::initializer_list<std::pair<std::string, value>> list);

public:
	iterator begin()              { return values_.begin(); }
	iterator end()                { return values_.end(); }
	const_iterator begin() const  { return values_.begin(); }
	const_iterator end() const    { return values_.end(); }
	const_iterator cbegin() const { return values_.begin(); }
	const_iterator cend() const   { return values_.end(); }

public:
	iterator find(const std::string &s) {
		return std::find_if(values_.begin(), values_.end(), [&s](const std::pair<std::string, value> &entry) {
			return entry.first == s;
		});
	}

	const_iterator find(const std::string &s) const {
		return std::find_if(values_.begin(), values_.end(), [&s](const std::pair<std::string, value> &entry) {
			return entry.first == s;
		});
	}

public:
	size_type size() const     { return values_.size(); }
	size_type max_size() const { return values_.max_size(); }
	bool empty() const         { return values_.empty(); }

public:
	const value operator[](const std::string &key) const;
	value &operator[](const std::string &key);

	const value at(const std::string &key) const;
	value &at(const std::string &key);

	template <class T>
	object &insert(const std::string &key, const T &v);

	template <class T>
	object &insert(const std::pair<std::string, T> &p);

	template <class T>
	object &insert(std::pair<std::string, T> &&p);
public:
	void swap(object &other);

private:
	C values_;
};

bool operator==(const object &lhs, const object &rhs);
bool operator!=(const object &lhs, const object &rhs);

inline object::iterator begin(object &obj)              { return obj.begin(); }
inline object::iterator end(object &obj)                { return obj.end();   }
inline object::const_iterator begin(const object &obj)  { return obj.begin(); }
inline object::const_iterator end(const object &obj)    { return obj.end();   }
inline object::const_iterator cbegin(const object &obj) { return obj.begin(); }
inline object::const_iterator cend(const object &obj)   { return obj.end();   }

}

#endif

