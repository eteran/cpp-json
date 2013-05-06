
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
		 template<class In> friend boost::shared_ptr<array> detail::get_array(In &, const In &);
		 friend size_t size(const array &a);

	public:
		array();
		array(const array &other);
		array &operator=(const array &rhs);
		
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

