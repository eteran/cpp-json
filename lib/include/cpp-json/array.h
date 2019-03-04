
#ifndef ARRAY_20110526_H_
#define ARRAY_20110526_H_

namespace json {

class array;
class value;

class array {
	friend bool operator==(const array &lhs, const array &rhs);
	friend bool operator!=(const array &lhs, const array &rhs);

	template <class In>
	friend class parser;

private:
	using C = std::vector<value>;

public:
	using allocator_type         = typename C::allocator_type;
	using reference              = typename C::reference;
	using const_reference        = typename C::const_reference;
	using pointer                = typename C::pointer;
	using const_pointer          = typename C::const_pointer;
	using iterator               = typename C::iterator;
	using const_iterator         = typename C::const_iterator;
	using reverse_iterator       = typename C::reverse_iterator;
	using const_reverse_iterator = typename C::const_reverse_iterator;
	using difference_type        = typename C::difference_type;
	using size_type              = typename C::size_type;

public:
	array()                            = default;
	array(array &&other)               = default;
	array(const array &other)          = default;
	array &operator=(array &&rhs)      = default;
	array &operator=(const array &rhs) = default;
	array(std::initializer_list<value> list);

public:
	iterator begin()                       { return values_.begin();  }
	iterator end()                         { return values_.end();    }
	const_iterator begin() const           { return values_.begin();  }
	const_iterator end() const             { return values_.end();    }
	const_iterator cbegin() const          { return values_.begin();  }
	const_iterator cend() const            { return values_.end();    }
	reverse_iterator rbegin()              { return values_.rbegin(); }
	reverse_iterator rend()                { return values_.rend();   }
	const_reverse_iterator rbegin() const  { return values_.rbegin(); }
	const_reverse_iterator rend() const    { return values_.rend();   }
	const_reverse_iterator crbegin() const { return values_.rbegin(); }
	const_reverse_iterator crend() const   { return values_.rend();   }

public:
	size_type size() const     { return values_.size();     }
	size_type max_size() const { return values_.max_size(); }
	bool empty() const         { return values_.empty();    }

public:
	const value operator[](std::size_t n) const;
	value &operator[](std::size_t n);

	const value at(std::size_t n) const;
	value &at(std::size_t n);

public:
	template <class T, class... Args>
	array &append(const T &v, Args &&...args);

	template <class T>
	array &append(T &&v);

	template <class T>
	array &append(const T &v);

public:
	void swap(array &other);

private:
	C values_;
};

bool operator==(const array &lhs, const array &rhs);
bool operator!=(const array &lhs, const array &rhs);

inline array::iterator begin(array &arr)              { return arr.begin(); }
inline array::iterator end(array &arr)                { return arr.end();   }
inline array::const_iterator begin(const array &arr)  { return arr.begin(); }
inline array::const_iterator end(const array &arr)    { return arr.end();   }
inline array::const_iterator cbegin(const array &arr) { return arr.begin(); }
inline array::const_iterator cend(const array &arr)   { return arr.end();   }

inline array::reverse_iterator rbegin(array &arr)              { return arr.rbegin(); }
inline array::reverse_iterator rend(array &arr)                { return arr.rend();   }
inline array::const_reverse_iterator rbegin(const array &arr)  { return arr.rbegin(); }
inline array::const_reverse_iterator rend(const array &arr)    { return arr.rend();   }
inline array::const_reverse_iterator crbegin(const array &arr) { return arr.rbegin(); }
inline array::const_reverse_iterator crend(const array &arr)   { return arr.rend();   }

}

#endif

