
#ifndef JSON_OBJECT_20110526_H_
#define JSON_OBJECT_20110526_H_

#include "json_value.h"
#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <string>

namespace json {

	class json_object;
	
	namespace detail {		
		template <class In>
		boost::shared_ptr<json_object> get_object(In &it, const In &last);
	}

	class json_object {
		template<class In> friend boost::shared_ptr<json_object> detail::get_object(In &, const In &);
		friend boost::unordered_set<std::string> json::keys(const json_value &);
		friend bool json::has_key(const json_value &v, const std::string &key);
		friend size_t size(const json_value &v);
		
	public:
		json_object();
		json_object(const json_object &other);
		json_object &operator=(const json_object &rhs);
		
	private:
		typedef boost::unordered_map<std::string, json_value> map_type;
		
	public:
		json_value operator[](const std::string &key) const;
		json_object &append(const std::string &key, const json_value &value);
		
	public:
		void swap(json_object &other);
		
	private:
		map_type values_;
	};
}

#endif

