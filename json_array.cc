
#include "json_array.h"
#include "json_value.h"

json::json_value json::json_array::operator[](std::size_t n) const {
	if(n >= values_.size()) {
		return json_value();
	}
	
	return values_[n];
}

json::json_array &json::json_array::append(const json_value &value) {
	values_.push_back(value);
	return *this;
}
