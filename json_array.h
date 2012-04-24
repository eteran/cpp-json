
#ifndef JSON_ARRAY_20110526_H_
#define JSON_ARRAY_20110526_H_

#include "json_value.h"
#include <boost/shared_ptr.hpp>
#include <vector>

namespace json {
	
	class json_array;
	
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
		json_value operator[](std::size_t n) const;
		json_array &append(const json_value &value);
		
	public:
		void swap(json_array &other);
	
	private:
		std::vector<json_value> values_;
	};
}


#endif

