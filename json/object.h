
#ifndef OBJECT_20110526_H_
#define OBJECT_20110526_H_

namespace json {

	class object;
	class value;
	
	namespace detail {		
		template <class In>
		boost::shared_ptr<object> get_object(In &it, const In &last);
	}

	class object {
		template<class In> friend boost::shared_ptr<object> detail::get_object(In &, const In &);
		
		friend bool has_key(const object &o, const std::string &key);
		friend set_type keys(const object &o);
		friend size_t size(const object &o);

	public:
		object();
		object(const object &other);
		object &operator=(const object &rhs);
		
	public:
		const value operator[](const std::string &key) const;
		value &operator[](const std::string &key);
		
		template <class T>
		object &insert(const std::string &key, const T &v);
		
	public:
		void swap(object &other);
		
	private:
		map_type values_;
	};
}

#endif

