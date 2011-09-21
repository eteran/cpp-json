
#include "json_object.h"
#include "json_value.h"
#include <boost/make_shared.hpp>

boost::shared_ptr<json::json_value> json::json_object::operator[](const std::string &key) const {
	boost::unordered_map<std::string, boost::shared_ptr<json_value> >::const_iterator it = values_.find(key);
	if(it != values_.end()) {
		return it->second;
	}

	return boost::make_shared<json_value>();
}
