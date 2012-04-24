
#ifndef JSON_ARRAY_20110526_H_
#define JSON_ARRAY_20110526_H_

namespace json {
	
	class json_array;
	class json_value;
	
	namespace detail {
		template <class In>
		boost::shared_ptr<json_array> get_array(In &it, const In &last);
	}
	
	class json_array {
		 template<class In> friend boost::shared_ptr<json_array> detail::get_array(In &, const In &);
		 friend size_t size(const json_value &v);

	public:
		json_array();
		json_array(const json_array &other);
		json_array &operator=(const json_array &rhs);
		
	public:
		const json_value operator[](std::size_t n) const;
		
		template <class T>
		json_array &append(const T &value);
		
	public:
		void swap(json_array &other);
	
	private:
		std::vector<json_value> values_;
	};
}


#endif

