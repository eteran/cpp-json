
#ifndef ARRAY_20110526_H_
#define ARRAY_20110526_H_

namespace json {
	
class array;
class value;

namespace detail {
	template <class In>
	boost::shared_ptr<array> get_array(In &it, const In &last);
}

class array {
	 template<class In>
	 friend boost::shared_ptr<array> detail::get_array(In &, const In &);
	 friend size_t size(const array &a);

public:
	array();
	array(const array &other);
#if __cplusplus >= 201103L
	template <class... Args>
    array(Args &&...args);
#endif
	array &operator=(const array &rhs);

private:
#if __cplusplus >= 201103L
	template <class T, class... Args>
    void array_init(const T &value, Args &&...args);

	template <class T>
    void array_init(const T &value);
#endif

public:
	const value operator[](std::size_t n) const;
	value &operator[](std::size_t n);

	template <class T>
	array &append(const T &v);

public:
	void swap(array &other);

private:
	std::vector<value> values_;
};

}

#endif

