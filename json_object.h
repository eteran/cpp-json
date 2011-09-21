
#ifndef JSON_OBJECT_20110526_H_
#define JSON_OBJECT_20110526_H_

#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <string>

namespace json {

	class json_value;
	class json_object;

	namespace detail {
		template <class In>
		boost::shared_ptr<json_value> get_value(In &it, const In &last);
		
		template <class In>
		boost::shared_ptr<json_object> get_object(In &it, const In &last);
	}

	class json_object {
		template<class In> friend boost::shared_ptr<json_value>  detail::get_value(In &, const In &);
		template<class In> friend boost::shared_ptr<json_object> detail::get_object(In &, const In &);
		
	public:
		boost::shared_ptr<json_value> operator[](const std::string &key) const;
		
	private:
		boost::unordered_map<std::string, boost::shared_ptr<json_value> > values_;
	};
}

#endif

