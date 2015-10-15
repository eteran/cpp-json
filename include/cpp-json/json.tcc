
#ifndef JSON_20110526_TCC_
#define JSON_20110526_TCC_

namespace json {
namespace detail {

template <class In, class Tr>
int distance_in_stream_internal(In first, In current, const Tr&) {
	(void)first;
	(void)current;
	return -1;
}

template <class In>
int distance_in_stream_internal(In first, In current, const std::random_access_iterator_tag &) {
	return std::distance(first, current);
}

template <class In>
int distance_in_stream(In first, In current) {
	typedef typename std::iterator_traits<In>::iterator_category Cat;
	return distance_in_stream_internal(first, current, Cat());
}

}

template <class In>
value parse(In first, In last) {

	parser<In> p(first, last);

	try {
		return p.parse();
	} catch(exception &e) {
		e.location = detail::distance_in_stream(p.begin(), p.current());
		throw;
	}
}

inline std::string to_string(const value &v) {
	return as_string(v);
}

inline bool to_bool(const value &v) {
	if(!is_bool(v)) {
		throw invalid_type_cast();
	}

	return v.as_string() == "true";
}

inline double to_number(const value &v) {
	if(!is_number(v)) {
		throw invalid_type_cast();
	}

	return stod(as_string(v), 0);
}

inline object to_object(const value &v) {
	return as_object(v);
}

inline array to_array(const value &v) {
	return as_array(v);
}

inline object &as_object(value &v) {
	if(!is_object(v)) {
		throw invalid_type_cast();
	}

	return v.as_object();
}

inline const object &as_object(const value &v) {
	if(!is_object(v)) {
		throw invalid_type_cast();
	}

	return v.as_object();
}

inline array &as_array(value &v) {
	if(!is_array(v)) {
		throw invalid_type_cast();
	}

	return v.as_array();
}

inline const array &as_array(const value &v) {
	if(!is_array(v)) {
		throw invalid_type_cast();
	}

	return v.as_array();
}

const std::string &as_string(const value &v) {
	if(!is_string(v) && !is_bool(v) && !is_number(v) && !is_null(v)) {
		throw invalid_type_cast();
	}

	return v.as_string();
}

std::string &as_string(value &v) {
	if(!is_string(v) && !is_bool(v) && !is_number(v) && !is_null(v)) {
		throw invalid_type_cast();
	}

	return v.as_string();
}

inline bool has_key(const value &v, const std::string &key) {
	if(is_object(v)) {
		return has_key(as_object(v), key);
	}
	return false;
}

inline bool has_key(const object &o, const std::string &key) {
	return o.find(key) != o.end();
}

inline value parse(std::istream &&is) {
	return parse(is);
}

inline value parse(std::wistream &&is) {
	return parse(is);
}

inline value parse(std::istream &is) {
	return parse((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
}

inline value parse(std::wistream &is) {
	return parse((std::istreambuf_iterator<wchar_t>(is)), std::istreambuf_iterator<wchar_t>());
}

inline value parse(const std::string &s) {
	return parse(s.begin(), s.end());
}

inline value parse(const std::wstring &s) {
	return parse(s.begin(), s.end());
}

inline bool is_string(const value &v) { return (v.type_ == value::type_string); }
inline bool is_bool(const value &v)   { return (v.type_ == value::type_boolean); }
inline bool is_number(const value &v) { return (v.type_ == value::type_number); }
inline bool is_object(const value &v) { return (v.type_ == value::type_object); }
inline bool is_array(const value &v)  { return (v.type_ == value::type_array); }
inline bool is_null(const value &v)   { return (v.type_ == value::type_null); }

namespace detail {

inline std::string escape_string(const std::string &s, unsigned options) {

	std::string r;
	r.reserve(s.size());

	if(options & ESCAPE_UNICODE) {
		typedef struct {
			unsigned int expected : 4,
                		 seen     : 4,
                		 reserved : 24;
		} state_t;

		state_t shift_state = {0,0,0};
		uint32_t result     = 0;

		for(std::string::const_iterator it = s.begin(); it != s.end(); ++it) {

			const unsigned char ch = *it;

			if(shift_state.seen == 0) {

				if((ch & 0x80) == 0) {
					switch(*it) {
					case '\"': r += "\\\""; break;
					case '\\': r += "\\\\"; break;
				#if 0
					case '/':  r += "\\/"; break;
				#endif
					case '\b': r += "\\b"; break;
					case '\f': r += "\\f"; break;
					case '\n': r += "\\n"; break;
					case '\r': r += "\\r"; break;
					case '\t': r += "\\t"; break;
					default:
						r += *it;
						break;
					}
				}else if((ch & 0xe0) == 0xc0) {
					// 2 byte
					result = ch & 0x1f;
					shift_state.expected = 2;
					shift_state.seen     = 1;
				} else if((ch & 0xf0) == 0xe0) {
					// 3 byte
					result = ch & 0x0f;
					shift_state.expected = 3;
					shift_state.seen     = 1;
				} else if((ch & 0xf8) == 0xf0) {
					// 4 byte
					result = ch & 0x07;
					shift_state.expected = 4;
					shift_state.seen     = 1;
				} else if((ch & 0xfc) == 0xf8) {
					// 5 byte
					throw invalid_utf8_string(); // Restricted by RFC 3629
				} else if((ch & 0xfe) == 0xfc) {
					// 6 byte
					throw invalid_utf8_string(); // Restricted by RFC 3629
				} else {
					throw invalid_utf8_string(); // should never happen
				}
			} else if(shift_state.seen < shift_state.expected) {
				if((ch & 0xc0) == 0x80) {
					result <<= 6;
					result |= ch & 0x3f;
					// increment the shift state
					++shift_state.seen;

					if(shift_state.seen == shift_state.expected) {
						// done with this character

						char buf[5];

						if(result < 0xd800 || (result >= 0xe000 && result < 0x10000)) {
							r += "\\u";
							snprintf(buf, sizeof(buf), "%04X", result);
							r += buf;
						} else {
							result = (result - 0x10000);

							r += "\\u";
							snprintf(buf, sizeof(buf), "%04X", 0xd800 + ((result >> 10) & 0x3ff));
							r += buf;

							r += "\\u";
							snprintf(buf, sizeof(buf), "%04X", 0xdc00 + (result & 0x3ff));
							r += buf;
						}

						shift_state.seen     = 0;
						shift_state.expected = 0;
						result = 0;
					}

				} else {
					throw invalid_utf8_string(); // should never happen
				}
			} else {
				throw invalid_utf8_string(); // should never happen
			}
		}
	} else {

		for(std::string::const_iterator it = s.begin(); it != s.end(); ++it) {

			switch(*it) {
			case '\"': r += "\\\""; break;
			case '\\': r += "\\\\"; break;
		#if 0
			case '/':  r += "\\/"; break;
		#endif
			case '\b': r += "\\b"; break;
			case '\f': r += "\\f"; break;
			case '\n': r += "\\n"; break;
			case '\r': r += "\\r"; break;
			case '\t': r += "\\t"; break;
			default:
				r += *it;
				break;
			}
		}
	}
	return r;
}

inline std::string escape_string(const std::string &s) {
	return escape_string(s, 0);
}

inline std::string value_to_string(const value &v, unsigned options, int indent, bool ignore_initial_ident) {

	static const int indent_width = 2;

	std::stringstream ss;

	if(!ignore_initial_ident) {
		ss << std::string(indent * indent_width, ' ');
	}

	if(is_string(v)) {
		ss << '"' << escape_string(as_string(v), options) << '"';
	}

	if(is_number(v)) {
		ss << as_string(v);
	}

	if(is_null(v)) {
		ss << as_string(v);
	}

	if(is_bool(v)) {
		ss << (to_bool(v) ? "true" : "false");
	}

	if(is_object(v)) {

		const object &o = as_object(v);

		ss << "{\n";
		if(!o.empty()) {

			object::const_iterator it = o.begin();
			object::const_iterator e  = o.end();

			++indent;
			ss << std::string(indent * indent_width, ' ') << '"' << escape_string(it->first, options) << "\" : " << value_to_string(it->second, options, indent, true);
			++it;
			for(;it != e; ++it) {
				ss << ',';
				ss << '\n';
				ss << std::string(indent * indent_width, ' ') << '"' << escape_string(it->first, options) << "\" : " << value_to_string(it->second, options, indent, true);
			}
			--indent;

		}
		ss << "\n";
		ss << std::string(indent * indent_width, ' ') << "}";
	}

	if(is_array(v)) {

		const array &a = as_array(v);

		ss << "[\n";
		if(!a.empty()) {

			array::const_iterator it = a.begin();
			array::const_iterator e  = a.end();

			++indent;
			ss << value_to_string(*it++, options, indent, false);

			for(;it != e; ++it) {
				ss << ',';
				ss << '\n';
				ss << value_to_string(*it, options, indent, false);
			}
			--indent;

		}
		ss << "\n";
		ss << std::string(indent * indent_width, ' ') << "]";
	}

	return ss.str();
}

inline std::string value_to_string(const value &v, unsigned options) {
	return value_to_string(v, options, 0, false);
}

inline std::string serialize(const value &v, unsigned options) {

	std::stringstream ss;

	if(is_string(v)) {
		ss << '"' << escape_string(as_string(v), options) << '"';
	}

	if(is_number(v)) {
		ss << as_string(v);
	}

	if(is_null(v)) {
		ss << as_string(v);
	}

	if(is_bool(v)) {
		ss << (to_bool(v) ? "true" : "false");
	}

	if(is_object(v)) {
		const object &o = as_object(v);
		ss << "{";
		if(!o.empty()) {
			object::const_iterator it = o.begin();
			object::const_iterator e  = o.end();

			ss << '"' << escape_string(it->first, options) << "\":" << serialize(it->second, options);
			++it;
			for(;it != e; ++it) {
				ss << ',';
				ss << '"' << escape_string(it->first, options) << "\":" << serialize(it->second, options);
			}
		}
		ss  << "}";
	}

	if(is_array(v)) {
		const array &a = as_array(v);

		ss << "[";
		if(!a.empty()) {
			array::const_iterator it = a.begin();
			array::const_iterator e  = a.end();

			ss << serialize(*it++, options);

			for(;it != e; ++it) {
				ss << ',';
				ss << serialize(*it, options);
			}
		}
		ss << "]";
	}

	return ss.str();
}

inline std::string serialize(const array &a, unsigned options) {
	return serialize(value(a), options);
}

inline std::string serialize(const object &o, unsigned options) {
	return serialize(value(o), options);
}

inline std::string pretty_print(const value &v, unsigned options) {
	return value_to_string(v, options);
}

inline std::string pretty_print(const array &a, unsigned options) {
	return value_to_string(value(a), options);
}

inline std::string pretty_print(const object &o, unsigned options) {
	return value_to_string(value(o), options);
}
}

inline std::string stringify(const value &v, unsigned options) {
	if(options & PRETTY_PRINT) {
		return detail::pretty_print(v, options);
	} else {
		return detail::serialize(v, options);
	}
}

inline std::string stringify(const array &a, unsigned options) {
	if(options & PRETTY_PRINT) {
		return detail::pretty_print(a, options);
	} else {
		return detail::serialize(a, options);
	}
}

inline std::string stringify(const object &o, unsigned options) {
	if(options & PRETTY_PRINT) {
		return detail::pretty_print(o, options);
	} else {
		return detail::serialize(o, options);
	}
}

inline std::string stringify(const value &v) {
	return stringify(v, 0);
}

inline std::string stringify(const array &a) {
	return stringify(a, 0);
}

inline std::string stringify(const object &o) {
	return stringify(o, 0);
}

}

#endif
