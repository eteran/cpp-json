
#ifndef JSON_OBJECT_20110526_H_
#define JSON_OBJECT_20110526_H_

namespace json {

	class json_object;
	class json_value;
	
	namespace detail {		
		template <class In>
		boost::shared_ptr<json_object> get_object(In &it, const In &last);
	}

	class json_object {
		template<class In> friend boost::shared_ptr<json_object> detail::get_object(In &, const In &);
		
		friend bool has_key(const json_object &o, const std::string &key);
		friend bool has_key(const json_value &v, const std::string &key);
		friend set_type keys(const json_value &v);
		friend set_type keys(const json_object &o);
		friend size_t size(const json_object &o);
		friend size_t size(const json_value &v);		

	public:
		json_object();
		json_object(const json_object &other);
		json_object &operator=(const json_object &rhs);
		
	public:
		const json_value operator[](const std::string &key) const;
		
		template <class T>
		json_object &append(const std::string &key, const T &value);
		
	public:
		void swap(json_object &other);
		
	private:
		map_type values_;
	};
}

#endif

