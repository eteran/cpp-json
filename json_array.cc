
#include "json_array.h"
#include "json_value.h"
#include <boost/make_shared.hpp>

boost::shared_ptr<json::json_value> json::json_array::operator[](std::size_t n) const {
	if(n >= values_.size()) {
		return boost::make_shared<json_value>();
	}
	
	return values_[n];
}
