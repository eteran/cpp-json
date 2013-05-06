
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
value get_value(In &it, const In &last);

std::vector<uint8_t> unicode_escape_to_utf8(uint16_t w1, uint16_t w2);

template <class In>
typename std::iterator_traits<In>::value_type peek_char(In &it, const In &last);

template <class In>
detail::token get_number(In &it, const In &last);

template <class In>
std::pair<std::string, value> get_pair(In &it, const In &last);

template <class In>
detail::token get_token(In &it, const In &last);

template <class In>
detail::token get_string(In &it, const In &last);

template <class In>
detail::token get_null(In &it, const In &last);

template <class In>
detail::token get_true(In &it, const In &last);

template <class In>
detail::token get_false(In &it, const In &last);

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

template <class Ch>
unsigned int to_hex(Ch ch) {


	static const int hexval[256] = {
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
		0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
		0x0, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
		0x0, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
	};

	if(static_cast<unsigned int>(ch) < 256) {
		return hexval[static_cast<unsigned int>(ch)];
	} else {
		return 0;
	}
}

template <class In>
boost::shared_ptr<object> get_object(In &it, const In &last) {
	boost::shared_ptr<object> obj = boost::make_shared<object>();

	detail::token tok = get_token(it, last);
	if(tok != detail::token("{", detail::token::type_delimeter)) {
		throw brace_expected();
	}

	// handle empty object
	if(peek_char(it, last) == '}') {
		tok = get_token(it, last);
	} else {

		while(true) {
			obj->values_.insert(get_pair(it, last));

			tok = get_token(it, last);
			if(tok != detail::token(",", detail::token::type_delimeter)) {
				break;
			}
		}
	}


	if(tok != detail::token("}", detail::token::type_delimeter)) {
		throw brace_expected();
	}

	return obj;
}

template <class In>
boost::shared_ptr<array> get_array(In &it, const In &last) {
	boost::shared_ptr<array> arr = boost::make_shared<array>();

	detail::token tok = get_token(it, last);
	if(tok != detail::token("[", detail::token::type_delimeter)) {
		throw bracket_expected();
	}

	// handle empty object
	if(peek_char(it, last) == ']') {
		tok = get_token(it, last);
	} else {
		while(true) {
			arr->values_.push_back(get_value(it, last));

			tok = get_token(it, last);
			if(tok != detail::token(",", detail::token::type_delimeter)) {
				break;
			}
		}
	}

	if(tok != detail::token("]", detail::token::type_delimeter)) {
		throw bracket_expected();
	}

	return arr;
}

template <class In>
value get_value(In &it, const In &last) {
	switch(peek_char(it, last)) {
	case '{': return value(get_object(it, last));
	case '[': return value(get_array(it, last));
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
		return value(get_number(it, last));
	case '"':
		return value(get_string(it, last));
	case 't':
		return value(get_true(it, last));
	case 'f':
		return value(get_false(it, last));
	case 'n':
		return value(get_null(it, last));
	}

	throw value_expected();
}

template <class In>
std::pair<std::string, value> get_pair(In &it, const In &last) {
	detail::token tok = get_token(it, last);
	if(tok.type_ != detail::token::type_string) {
		throw string_expected();
	}

	const std::string key = tok.string_;

	tok = get_token(it, last);
	if(tok != detail::token(":", detail::token::type_delimeter)) {
		throw colon_expected();
	}

	return std::make_pair(key, get_value(it, last));
}

template <class In>
detail::token get_token(In &it, const In &last) {
	if(it != last) {
		switch(peek_char(it, last)) {
		case '{': ++it; return detail::token("{", detail::token::type_delimeter);
		case '}': ++it; return detail::token("}", detail::token::type_delimeter);
		case '[': ++it; return detail::token("[", detail::token::type_delimeter);
		case ']': ++it; return detail::token("]", detail::token::type_delimeter);
		case ':': ++it; return detail::token(":", detail::token::type_delimeter);
		case ',': ++it; return detail::token(",", detail::token::type_delimeter);
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
	return detail::token();
}

template <class In>
detail::token get_string(In &it, const In &last) {
	if(*it != '"') {
		throw string_expected();
	}
	++it;

	std::string s;
	std::back_insert_iterator<std::string> out = back_inserter(s);

	while(it != last && *it != '"' && *it != '\n') {
		if(*it == '\\') {
			++it;
			if(it != last) {
				switch(*it) {
				case '"':  *out++ = '"'; break;
				case '\\': *out++ = '\\'; break;
				case '/':  *out++ = '/'; break;
				case 'b':  *out++ = '\b'; break;
				case 'f':  *out++ = '\f'; break;
				case 'n':  *out++ = '\n'; break;
				case 'r':  *out++ = '\r'; break;
				case 't':  *out++ = '\t'; break;
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
						
						w1 |= (to_hex(hex[0]) << 12);
						w1 |= (to_hex(hex[1]) << 8);
						w1 |= (to_hex(hex[2]) << 4);
						w1 |= (to_hex(hex[3]));
						
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
							
							w2 |= (to_hex(hex[0]) << 12);
							w2 |= (to_hex(hex[1]) << 8);
							w2 |= (to_hex(hex[2]) << 4);
							w2 |= (to_hex(hex[3]));							
						}

						const std::vector<uint8_t> utf8 = unicode_escape_to_utf8(w1, w2);
						std::copy(utf8.begin(), utf8.end(), out);
					}
					break;

				default:
					*out++ = '\\';
					break;
				}
			}
		} else {
			*out++ = *it;
		}
		++it;
	}

	if(*it != '"' || it == last) {
		throw quote_expected();
	}

	++it;

	return detail::token(s, detail::token::type_string);
}

template <class In>
detail::token get_number(In &it, const In &last) {
	std::string s;
	std::back_insert_iterator<std::string> out = back_inserter(s);
	
	// JSON numbers fit the regex: -?(0|[1-9][0-9]*)(\.[0-9]+)?([eE][+-]?[0-9]+)?

	// -?
	if(it != last && *it == '-') {
		*out++ = *it++;
	}

	// (0|[1-9][0-9]*)
	if(it != last) {
		if(*it >= '1' && *it <= '9') {
			while(it != last && is_digit(*it)) {
				*out++ = *it++;
			}
		} else if(*it == '0') {
			*out++ = *it++;
		}
	}

	// (\.[0-9]+)?
	if(it != last && *it == '.') {
		*out++ = *it++;
		if(!is_digit(*it)) {
			throw invalid_number();
		}

		while(it != last && is_digit(*it)) {
			*out++ = *it++;
		}
	}

	// ([eE][+-]?[0-9]+)?
	if(it != last && (*it == 'e' || *it == 'E')) {
		*out++ = *it++;
		if(it != last && (*it == '+' || *it == '-')) {
			*out++ = *it++;
		}
		if(!is_digit(*it)) {
			throw invalid_number();
		}
		while(it != last && is_digit(*it)) {
			*out++ = *it++;
		}
	}

	return detail::token(s, detail::token::type_number);
}

template <class In>
detail::token get_null(In &it, const In &last) {

	if(it == last || *it++ != 'n') { throw keyword_expected(); }
	if(it == last || *it++ != 'u') { throw keyword_expected(); }
	if(it == last || *it++ != 'l') { throw keyword_expected(); }
	if(it == last || *it++ != 'l') { throw keyword_expected(); }

	return detail::token("null", detail::token::type_null);	
}

template <class In>
detail::token get_true(In &it, const In &last) {

	if(it == last || *it++ != 't') { throw boolean_expected(); }
	if(it == last || *it++ != 'r') { throw boolean_expected(); }
	if(it == last || *it++ != 'u') { throw boolean_expected(); }
	if(it == last || *it++ != 'e') { throw boolean_expected(); }

	return detail::token("true", detail::token::type_boolean);	
}

template <class In>
detail::token get_false(In &it, const In &last) {

	if(it == last || *it++ != 'f') { throw boolean_expected(); }
	if(it == last || *it++ != 'a') { throw boolean_expected(); }
	if(it == last || *it++ != 'l') { throw boolean_expected(); }
	if(it == last || *it++ != 's') { throw boolean_expected(); }
	if(it == last || *it++ != 'e') { throw boolean_expected(); }

	return detail::token("false", detail::token::type_boolean);	
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
value parse(In first, In last) {
	const In original_first = first;
	try {
		return detail::get_value(first, last);
	} catch(const exception &e) {
		detail::print_exception_details(original_first, first, last);	
		throw;
	}
}

inline std::string to_string(const value &v) {
	if(!is_string(v) && !is_bool(v) && !is_number(v)) {
		throw invalid_type_cast();
	}
	return boost::get<std::string>(v.value_);
}

inline bool to_bool(const value &v) {
	if(!is_bool(v)) {
		throw invalid_type_cast();
	}
	return boost::get<std::string>(v.value_) == "true";
}

inline double to_number(const value &v) {
	if(!is_number(v)) {
		throw invalid_type_cast();
	}
	return strtod(boost::get<std::string>(v.value_).c_str(), 0);
}

inline object to_object(const value &v) {
	if(!is_object(v)) {
		throw invalid_type_cast();
	}
	
	return *boost::get<boost::shared_ptr<object> >(v.value_);
}

inline array to_array(const value &v) {
	if(!is_array(v)) {
		throw invalid_type_cast();
	}
	
	return *boost::get<boost::shared_ptr<array> >(v.value_);
}

inline size_t size(const value &v) {

	if(is_array(v)) {
		return size(*boost::get<boost::shared_ptr<array> >(v.value_));
	}
	
	if(is_object(v)) {
		return size(*boost::get<boost::shared_ptr<object> >(v.value_));
	}
	
	return 0;
}

inline bool has_key(const value &v, const std::string &key) {
	if(is_object(v)) {
		return has_key(*boost::get<boost::shared_ptr<object> >(v.value_), key);
	}
	return false;
}

inline set_type keys(const value &v) {
	
	if(is_object(v)) {
		return keys(*boost::get<boost::shared_ptr<object> >(v.value_));		
	}
	
	return set_type();
}

inline size_t size(const object &o) {
	return o.values_.size();
}

inline set_type keys(const object &o) {
	set_type keys;

	const map_type &map = o.values_;

	for(map_type::const_iterator it = map.begin(); it != map.end(); ++it) {
		keys.insert(it->first);
	}		
	
	return keys;
}

inline bool has_key(const object &o, const std::string &key) {
	return o.values_.find(key) != o.values_.end();
}

inline size_t size(const array &a) {
	return a.values_.size();
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

inline std::vector<uint8_t> detail::unicode_escape_to_utf8(uint16_t w1, uint16_t w2) {

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

	inline std::string escape_string(const std::string &s, int options) {
	
	
		std::string r;
		r.reserve(s.size());
	
		if(options & ESCAPE_UNICODE) {
			typedef struct {
				unsigned int expected : 4,
                			 seen     : 4,
                			 reserved : 12;
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
			//			case '/':  r += "\\/"; break;
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
	//			case '/':  r += "\\/"; break;
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

	inline std::string value_to_string(const json::value &v, int indent, bool ignore_initial_ident, int options) {
		std::stringstream ss;
		
		if(!ignore_initial_ident) {
			ss << std::string(indent * 2, ' ');
		}
		
		if(is_string(v)) {
			ss << '"' << escape_string(to_string(v), options) << '"';
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
				ss << std::string(indent * 2, ' ') << '"' << escape_string(*it, options) << "\" : " << value_to_string(v[*it], indent, true, options);
				++it;
				for(;it != k.end(); ++it) {
					ss << ',';
					ss << '\n';
					ss << std::string(indent * 2, ' ') << '"' << escape_string(*it, options) << "\" : " << value_to_string(v[*it], indent, true, options);
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
				ss << value_to_string(v[i++], indent, false, options);

				for(;i != size(v); ++i) {
					ss << ',';
					ss << '\n';
					ss << value_to_string(v[i], indent, false, options);
				}
				--indent;

			}
			ss << "\n";
			ss << std::string(indent * 2, ' ') << "]";
		}

		return ss.str();
	}
}

inline std::string pretty_print(const value &v, int options) {
	return value_to_string(v, 0, false, options);
}

inline std::string pretty_print(const array &a, int options) {
	return value_to_string(value(a), 0, false, options);
}

inline std::string pretty_print(const object &o, int options) {
	return value_to_string(value(o), 0, false, options);
}

inline std::string pretty_print(const value &v) {
	return pretty_print(v, 0);
}

inline std::string pretty_print(const array &a) {
	return pretty_print(a, 0);
}

inline std::string pretty_print(const object &o) {
	return pretty_print(o, 0);
}

inline std::string serialize(const value &v, int options) {
	
	std::stringstream ss;

	if(is_string(v)) {
		ss << '"' << escape_string(to_string(v), options) << '"';
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
		set_type k = keys(v);
		if(!k.empty()) {
			set_type::const_iterator it = k.begin();
			ss << '"' << escape_string(*it, options) << "\":" << serialize(v[*it], options);
			++it;
			for(;it != k.end(); ++it) {
				ss << ',';
				ss << '"' << escape_string(*it, options) << "\":" << serialize(v[*it], options);
			}
		}
		ss  << "}";
	}

	if(is_array(v)) {
		ss << "[";
		if(size(v) != 0) {
			size_t i = 0;
			ss << serialize(v[i++], options);

			for(;i != size(v); ++i) {
				ss << ',';
				ss << serialize(v[i], options);
			}
		}
		ss << "]";
	}

	return ss.str();
}

inline std::string serialize(const array &a, int options) {
	return serialize(value(a), options);
}

inline std::string serialize(const object &o, int options) {
	return serialize(value(o), options);
}

inline std::string serialize(const object &o) {
	return serialize(o, 0);
}

inline std::string serialize(const array &a) {
	return serialize(a, 0);	
}

inline std::string serialize(const value &v) {
	return serialize(v, 0);
}

}

#endif
