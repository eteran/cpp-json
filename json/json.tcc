
#ifndef JSON_20110526_TCC_
#define JSON_20110526_TCC_

namespace json {
namespace detail {

template <class In, class Tr>
void print_exception_details_internal(In first, In current, In last, const Tr&) {
	(void)first;
	(void)current;
	(void)last;
}

template <class In>
void print_exception_details_internal(In first, In current, In last, const std::random_access_iterator_tag &) {
	(void)last;
	std::cerr << "an error occured " << std::distance(first, current) << " characters into the stream:" << std::endl;
}

template <class In>
void print_exception_details(In first, In current, In last) {
	typedef typename std::iterator_traits<In>::iterator_category Cat;
	print_exception_details_internal(first, current, last, Cat());
}

template <class In>
json_value get_value(In &it, const In &last);

std::vector<uint8_t> unicode_escape_to_utf8(uint16_t w1, uint16_t w2);

template <class In>
typename std::iterator_traits<In>::value_type peek_char(In &it, const In &last);

template <class In>
json_token get_number(In &it, const In &last);

template <class In>
std::pair<std::string, json_value> get_pair(In &it, const In &last);

template <class In>
json_token get_token(In &it, const In &last);

template <class In>
json_token get_string(In &it, const In &last);

template <class In>
json_token get_null(In &it, const In &last);

template <class In>
json_token get_true(In &it, const In &last);

template <class In>
json_token get_false(In &it, const In &last);

template <class Ch>
bool is_digit(Ch ch) {
	return std::isdigit(ch);
}

template <class Ch>
bool is_hexdigit(Ch ch) {
	return std::isxdigit(ch);
}

template <class Ch>
bool is_space(Ch ch) {
	return std::isspace(ch);
}

template <class In>
boost::shared_ptr<json_object> get_object(In &it, const In &last) {
	boost::shared_ptr<json_object> object = boost::make_shared<json_object>();

	json_token token = get_token(it, last);
	if(token != json_token("{", json_token::delimeter)) {
		throw brace_expected();
	}

	// handle empty object
	if(peek_char(it, last) == '}') {
		token = get_token(it, last);
	} else {

		while(true) {
			object->values_.insert(get_pair(it, last));

			token = get_token(it, last);
			if(token != json_token(",", json_token::delimeter)) {
				break;
			}
		}
	}


	if(token != json_token("}", json_token::delimeter)) {
		throw brace_expected();
	}

	return object;
}

template <class In>
boost::shared_ptr<json_array> get_array(In &it, const In &last) {
	boost::shared_ptr<json_array> array = boost::make_shared<json_array>();

	json_token token = get_token(it, last);
	if(token != json_token("[", json_token::delimeter)) {
		throw bracket_expected();
	}

	// handle empty object
	if(peek_char(it, last) == ']') {
		token = get_token(it, last);
	} else {
		while(true) {
			array->values_.push_back(get_value(it, last));

			token = get_token(it, last);
			if(token != json_token(",", json_token::delimeter)) {
				break;
			}
		}
	}

	if(token != json_token("]", json_token::delimeter)) {
		throw bracket_expected();
	}

	return array;
}

template <class In>
json_value get_value(In &it, const In &last) {
	switch(peek_char(it, last)) {
	case '{': return json_value(get_object(it, last));
	case '[': return json_value(get_array(it, last));
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case '-':
		return json_value(get_number(it, last));
	case '"':
		return json_value(get_string(it, last));
	case 't':
		return json_value(get_true(it, last));
	case 'f':
		return json_value(get_false(it, last));
	case 'n':
		return json_value(get_null(it, last));
	}

	throw value_expected();
}

template <class In>
std::pair<std::string, json_value> get_pair(In &it, const In &last) {
	json_token token = get_token(it, last);
	if(token.type_ != json_token::string) {
		throw string_expected();
	}

	const std::string key = token.string_;

	token = get_token(it, last);
	if(token != json_token(":", json_token::delimeter)) {
		throw colon_expected();
	}

	return std::make_pair(key, get_value(it, last));
}

template <class In>
json_token get_token(In &it, const In &last) {
	if(it != last) {
		switch(peek_char(it, last)) {
		case '{': ++it; return json_token("{", json_token::delimeter);
		case '}': ++it; return json_token("}", json_token::delimeter);
		case '[': ++it; return json_token("[", json_token::delimeter);
		case ']': ++it; return json_token("]", json_token::delimeter);
		case ':': ++it; return json_token(":", json_token::delimeter);
		case ',': ++it; return json_token(",", json_token::delimeter);
		case '"': return get_string(it, last);
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '-':
			return get_number(it, last);
		case 't':
			return get_true(it, last);
		case 'f':
			return get_false(it, last);
		default:
			throw keyword_expected();
		}
	}
	return json_token();
}

template <class In>
json_token get_string(In &it, const In &last) {
	if(*it != '"') {
		throw string_expected();
	}
	++it;


	std::string s;

	while(it != last && *it != '"' && *it != '\n') {
		if(*it == '\\') {
			++it;
			if(it != last) {
				switch(*it) {
				case '"': s += '"'; break;
				case '\\': s += '\\'; break;
				case '/': s += '/'; break;
				case 'b': s += '\b'; break;
				case 'f': s += '\f'; break;
				case 'n': s += '\n'; break;
				case 'r': s += '\r'; break;
				case 't': s += '\t'; break;
				case 'u':
					{
						// convert \uXXXX escape sequences to UTF-8
						char hex[4];
						if(it == last) { throw hex_character_expected(); } hex[0] = *++it;
						if(it == last) { throw hex_character_expected(); } hex[1] = *++it;
						if(it == last) { throw hex_character_expected(); } hex[2] = *++it;
						if(it == last) { throw hex_character_expected(); } hex[3] = *++it;

						if(!is_hexdigit(hex[0])) throw invalid_unicode_character();
						if(!is_hexdigit(hex[1])) throw invalid_unicode_character();
						if(!is_hexdigit(hex[2])) throw invalid_unicode_character();
						if(!is_hexdigit(hex[3])) throw invalid_unicode_character();

						uint16_t w1 = 0;
						uint16_t w2 = 0;
						
						w1 |= ((is_digit(hex[0]) ? hex[0] - '0' : std::toupper(hex[0]) - 'A' + 10) << 12);
						w1 |= ((is_digit(hex[1]) ? hex[1] - '0' : std::toupper(hex[1]) - 'A' + 10) << 8);
						w1 |= ((is_digit(hex[2]) ? hex[2] - '0' : std::toupper(hex[2]) - 'A' + 10) << 4);
						w1 |= ((is_digit(hex[3]) ? hex[3] - '0' : std::toupper(hex[3]) - 'A' + 10));
						
						if((w1 & 0xfc00) == 0xdc00) {
							throw invalid_unicode_character();
						}

						if((w1 & 0xfc00) == 0xd800) {
							// part of a surrogate pair
							if(it == last || *++it != '\\') { throw utf16_surrogate_expected(); }
							if(it == last || *++it != 'u')  { throw utf16_surrogate_expected(); }
							
							// convert \uXXXX escape sequences to UTF-8
							if(it == last) { throw hex_character_expected(); } hex[0] = *++it;
							if(it == last) { throw hex_character_expected(); } hex[1] = *++it;
							if(it == last) { throw hex_character_expected(); } hex[2] = *++it;
							if(it == last) { throw hex_character_expected(); } hex[3] = *++it;

							if(!is_hexdigit(hex[0])) throw invalid_unicode_character();
							if(!is_hexdigit(hex[1])) throw invalid_unicode_character();
							if(!is_hexdigit(hex[2])) throw invalid_unicode_character();
							if(!is_hexdigit(hex[3])) throw invalid_unicode_character();
							
							w2 |= ((is_digit(hex[0]) ? hex[0] - '0' : std::toupper(hex[0]) - 'A' + 10) << 12);
							w2 |= ((is_digit(hex[1]) ? hex[1] - '0' : std::toupper(hex[1]) - 'A' + 10) << 8);
							w2 |= ((is_digit(hex[2]) ? hex[2] - '0' : std::toupper(hex[2]) - 'A' + 10) << 4);
							w2 |= ((is_digit(hex[3]) ? hex[3] - '0' : std::toupper(hex[3]) - 'A' + 10));							
						}

						const std::vector<uint8_t> utf8 = unicode_escape_to_utf8(w1, w2);
						s.append(utf8.begin(), utf8.end());
					}
					break;

				default:
					s += '\\';
					break;
				}
			}
		} else {
			s += *it;
		}
		++it;
	}

	if(*it != '"' || it == last) {
		throw quote_expected();
	}

	++it;

	return json_token(s, json_token::string);
}

template <class In>
json_token get_number(In &it, const In &last) {
	std::string s;

	if(it != last && *it == '-') {
		s += *it++;
	}

	if(*it >= '1' && *it <= '9') {
		while(it != last && is_digit(*it)) {
			s += *it++;
		}
	} else if(*it == '0') {
		s += *it++;
	}

	if(it != last && *it == '.') {
		s += *it++;
		if(!is_digit(*it)) {
			return json_token();
		}

		while(it != last && is_digit(*it)) {
			s += *it++;
		}
	}

	if(it != last && (*it == 'e' || *it == 'E')) {
		s += *it++;
		if(it != last && (*it == '+' || *it == '-')) {
			s += *it++;
		}
		if(!is_digit(*it)) {
			return json_token();
		}
		while(it != last && is_digit(*it)) {
			s += *it++;
		}
	}

	return json_token(s, json_token::number);
}

template <class In>
json_token get_null(In &it, const In &last) {

	if(it == last || *it++ != 'n') { throw keyword_expected(); }
	if(it == last || *it++ != 'u') { throw keyword_expected(); }
	if(it == last || *it++ != 'l') { throw keyword_expected(); }
	if(it == last || *it++ != 'l') { throw keyword_expected(); }

	return json_token("null", json_token::null);	
}

template <class In>
json_token get_true(In &it, const In &last) {

	if(it == last || *it++ != 't') { throw boolean_expected(); }
	if(it == last || *it++ != 'r') { throw boolean_expected(); }
	if(it == last || *it++ != 'u') { throw boolean_expected(); }
	if(it == last || *it++ != 'e') { throw boolean_expected(); }

	return json_token("true", json_token::boolean);	
}

template <class In>
json_token get_false(In &it, const In &last) {

	if(it == last || *it++ != 'f') { throw boolean_expected(); }
	if(it == last || *it++ != 'a') { throw boolean_expected(); }
	if(it == last || *it++ != 'l') { throw boolean_expected(); }
	if(it == last || *it++ != 's') { throw boolean_expected(); }
	if(it == last || *it++ != 'e') { throw boolean_expected(); }

	return json_token("false", json_token::boolean);	
}

template <class In>
typename std::iterator_traits<In>::value_type peek_char(In &it, const In &last) {

	// first eat up some whitespace
	while(it != last && is_space(*it)) {
		++it;
	}

	return *it;
}
}
	
	
template <class In>
json_value parse(In first, In last) {
	const In original_first = first;
	try {
		return detail::get_value(first, last);
	} catch(const json_exception &e) {
		detail::print_exception_details(original_first, first, last);	
		throw;
	}
}

inline std::string to_string(const json_value &v) {
	if(!is_string(v) && !is_bool(v) && !is_number(v)) {
		throw invalid_type_cast();
	}
	return boost::get<std::string>(v.value_);
}

inline bool to_bool(const json_value &v) {
	if(!is_bool(v)) {
		throw invalid_type_cast();
	}
	return boost::get<std::string>(v.value_) == "true";
}

inline double to_number(const json_value &v) {
	if(!is_number(v)) {
		throw invalid_type_cast();
	}
	return strtod(boost::get<std::string>(v.value_).c_str(), 0);
}

inline json_object to_object(const json_value &v) {
	if(!is_object(v)) {
		throw invalid_type_cast();
	}
	
	return *boost::get<boost::shared_ptr<json_object> >(v.value_);
}

inline json_array to_array(const json_value &v) {
	if(!is_array(v)) {
		throw invalid_type_cast();
	}
	
	return *boost::get<boost::shared_ptr<json_array> >(v.value_);
}

inline size_t size(const json_value &v) {

	if(is_array(v)) {
		return boost::get<boost::shared_ptr<json_array> >(v.value_)->values_.size();
	}
	
	if(is_object(v)) {
		return boost::get<boost::shared_ptr<json_object> >(v.value_)->values_.size();
	}
	
	return 0;
}

inline bool has_key(const json_value &v, const std::string &key) {
	if(is_object(v)) {
		return boost::get<boost::shared_ptr<json_object> >(v.value_)->values_.find(key) != boost::get<boost::shared_ptr<json_object> >(v.value_)->values_.end();
	}
	return false;
}

inline set_type keys(const json_value &v) {
	
	set_type keys;
	if(is_object(v)) {
	
		const json::map_type &map = boost::get<boost::shared_ptr<json_object> >(v.value_)->values_;
	
		for(json::map_type::const_iterator it = map.begin(); it != map.end(); ++it) {
			keys.insert(it->first);
		}		
	}
	
	return keys;
}

inline size_t size(const json_object &o) {
	return o.values_.size();
}

inline set_type keys(const json_object &o) {
	set_type keys;

	const json::map_type &map = o.values_;

	for(json::map_type::const_iterator it = map.begin(); it != map.end(); ++it) {
		keys.insert(it->first);
	}		
	
	return keys;
}

inline bool has_key(const json_object &o, const std::string &key) {
	return o.values_.find(key) != o.values_.end();
}

inline size_t size(const json_array &a) {
	return a.values_.size();
}

inline json_value parse(std::istream &is) {
	return parse((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
}

inline json_value parse(const std::string &s) {
	return parse(s.begin(), s.end());
}

inline json_value from_string(const std::string &s) {
	return parse(s.begin(), s.end());
}

inline bool is_string(const json_value &v) { return (v.type_ == json_value::string); }
inline bool is_bool(const json_value &v)   { return (v.type_ == json_value::boolean); }
inline bool is_number(const json_value &v) { return (v.type_ == json_value::number); }
inline bool is_object(const json_value &v) { return (v.type_ == json_value::object); }
inline bool is_array(const json_value &v)  { return (v.type_ == json_value::array); }
inline bool is_null(const json_value &v)   { return (v.type_ == json_value::null); }

}

inline std::vector<uint8_t> json::detail::unicode_escape_to_utf8(uint16_t w1, uint16_t w2) {

	uint32_t cp;
	if((w1 & 0xfc00) == 0xd800) {
		if((w2 & 0xfc00) == 0xdc00) {
			cp = 0x10000 + (((static_cast<uint32_t>(w1) & 0x3ff) << 10) | (w2 & 0x3ff));
		} else {
			throw invalid_unicode_character();
		}
	} else {
		cp = w1;
	}
	
	std::vector<uint8_t> utf8;
	
	if(cp < 0x80) {
		utf8.push_back(static_cast<uint8_t>(cp));
	} else if(cp < 0x0800) {
		uint8_t ch[2];
		ch[0] = 0xc0 | ((cp >> 6) & 0x1f);
		ch[1] = 0x80 | (cp & 0x3f);
		utf8.push_back(static_cast<uint8_t>(ch[0]));
		utf8.push_back(static_cast<uint8_t>(ch[1]));
	} else if(cp < 0x10000) {
		uint8_t ch[3];
		ch[0] = 0xe0 | ((cp >> 6) & 0x0f);
		ch[1] = 0x80 | ((cp >> 6) & 0x3f);
		ch[2] = 0x80 | (cp & 0x3f);
		utf8.push_back(static_cast<uint8_t>(ch[0]));
		utf8.push_back(static_cast<uint8_t>(ch[1]));
		utf8.push_back(static_cast<uint8_t>(ch[2]));
	} else if(cp < 0x1fffff) {
		uint8_t ch[4];
		ch[0] = 0xf0 | ((cp >> 18) & 0x07);
		ch[1] = 0x80 | ((cp >> 12) & 0x3f);
		ch[2] = 0x80 | ((cp >> 6) & 0x3f);
		ch[3] = 0x80 | (cp & 0x3f);
		utf8.push_back(static_cast<uint8_t>(ch[0]));
		utf8.push_back(static_cast<uint8_t>(ch[1]));
		utf8.push_back(static_cast<uint8_t>(ch[2]));
		utf8.push_back(static_cast<uint8_t>(ch[3]));
	}
	
	return utf8;
}

namespace {
	inline std::string value_to_string(const json::json_value &v, int indent, bool ignore_initial_ident) {
		std::stringstream ss;
		
		if(!ignore_initial_ident) {
			ss << std::string(indent * 2, ' ');
		}
		
		if(is_string(v)) {
			ss << '"' << to_string(v) << '"';
		}

		if(is_number(v)) {
			ss << to_string(v);
		}

		if(is_null(v)) {
			ss << "null";
		}

		if(is_bool(v)) {
			ss << (to_bool(v) ? "true" : "false");
		}

		if(is_object(v)) {
			ss << "{\n";
			json::set_type k = keys(v);
			if(!k.empty()) {
				json::set_type::const_iterator it = k.begin();
				++indent;
				ss << std::string(indent * 2, ' ') << '"' << *it << "\" : " << value_to_string(v[*it], indent, true);
				++it;
				for(;it != k.end(); ++it) {
					ss << ',';
					ss << '\n';
					ss << std::string(indent * 2, ' ') << '"' << *it << "\" : " << value_to_string(v[*it], indent, true);
				}
				--indent;

			}
			ss << "\n";
			ss << std::string(indent * 2, ' ') << "}";
		}

		if(is_array(v)) {
			ss << "[\n";
			if(size(v) != 0) {
				size_t i = 0;
				++indent;
				ss << value_to_string(v[i++], indent, false);

				for(;i != size(v); ++i) {
					ss << ',';
					ss << '\n';
					ss << value_to_string(v[i], indent, false);
				}
				--indent;

			}
			ss << "\n";
			ss << std::string(indent * 2, ' ') << "]";
		}

		return ss.str();
	}
}

inline std::string json::pretty_print(const json_value &v) {
	return value_to_string(v, 0, false);
}

inline std::string json::pretty_print(const json_array &a) {
	return value_to_string(json_value(a), 0, false);
}

inline std::string json::pretty_print(const json_object &o) {
	return value_to_string(json_value(o), 0, false);
}

inline std::string json::print(const json_object &o) {
	return print(json_value(o));
}

inline std::string json::print(const json_array &o) {
	return print(json_value(o));
}

inline std::string json::print(const json_value &v) {
	std::stringstream ss;

	if(is_string(v)) {
		ss << '"' << to_string(v) << '"';
	}

	if(is_number(v)) {
		ss << to_string(v);
	}

	if(is_null(v)) {
		ss << "null";
	}

	if(is_bool(v)) {
		ss << (to_bool(v) ? "true" : "false");
	}

	if(is_object(v)) {
		ss << "{";
		json::set_type k = keys(v);
		if(!k.empty()) {
			json::set_type::const_iterator it = k.begin();
			ss << '"' << *it << "\":" << print(v[*it]);
			++it;
			for(;it != k.end(); ++it) {
				ss << ',';
				ss << '"' << *it << "\":" << print(v[*it]);
			}
		}
		ss  << "}";
	}

	if(is_array(v)) {
		ss << "[";
		if(size(v) != 0) {
			size_t i = 0;
			ss << print(v[i++]);

			for(;i != size(v); ++i) {
				ss << ',';
				ss << print(v[i]);
			}
		}
	}

	return ss.str();
}

#endif
