
#ifndef PARSER_20140115_TCC_
#define PARSER_20140115_TCC_

namespace json {
namespace detail {

//------------------------------------------------------------------------------
// Name: to_hex
//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
// Name: surrogate_pair_to_utf8
//------------------------------------------------------------------------------
template <class Out>
void surrogate_pair_to_utf8(uint16_t w1, uint16_t w2, Out &out) {

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

	if(cp < 0x80) {
		*out++ = static_cast<uint8_t>(cp);
	} else if(cp < 0x0800) {
		*out++ = static_cast<uint8_t>(0xc0 | ((cp >> 6) & 0x1f));
		*out++ = static_cast<uint8_t>(0x80 | (cp & 0x3f));
	} else if(cp < 0x10000) {
		*out++ = static_cast<uint8_t>(0xe0 | ((cp >> 12) & 0x0f));
		*out++ = static_cast<uint8_t>(0x80 | ((cp >> 6) & 0x3f));
		*out++ = static_cast<uint8_t>(0x80 | (cp & 0x3f));
	} else if(cp < 0x1fffff) {
		*out++ = static_cast<uint8_t>(0xf0 | ((cp >> 18) & 0x07));
		*out++ = static_cast<uint8_t>(0x80 | ((cp >> 12) & 0x3f));
		*out++ = static_cast<uint8_t>(0x80 | ((cp >> 6) & 0x3f));
		*out++ = static_cast<uint8_t>(0x80 | (cp & 0x3f));
	}
}
}

//------------------------------------------------------------------------------
// Name: parser
//------------------------------------------------------------------------------
template <class In>
parser<In>::parser(In first, In last) : begin_(first), cur_(first), end_(last) {
}

//------------------------------------------------------------------------------
// Name: parse
//------------------------------------------------------------------------------
template <class In>
value parser<In>::parse() {
	return get_value();
}

//------------------------------------------------------------------------------
// Name: peek
//------------------------------------------------------------------------------
template <class In>
char parser<In>::peek() {
	// first eat up some whitespace
	while(cur_ != end_ && std::isspace(*cur_)) {
		++cur_;
	}

	if(cur_ != end_) {
		return *cur_;
	}

	return '\0';
}

//------------------------------------------------------------------------------
// Name: get_false
//------------------------------------------------------------------------------
template <class In>
bool parser<In>::get_false() {
	if(cur_ == end_ || *cur_++ != 'f') { throw boolean_expected(); }
	if(cur_ == end_ || *cur_++ != 'a') { throw boolean_expected(); }
	if(cur_ == end_ || *cur_++ != 'l') { throw boolean_expected(); }
	if(cur_ == end_ || *cur_++ != 's') { throw boolean_expected(); }
	if(cur_ == end_ || *cur_++ != 'e') { throw boolean_expected(); }

	return false;
}

//------------------------------------------------------------------------------
// Name: get_true
//------------------------------------------------------------------------------
template <class In>
bool parser<In>::get_true() {
	if(cur_ == end_ || *cur_++ != 't') { throw boolean_expected(); }
	if(cur_ == end_ || *cur_++ != 'r') { throw boolean_expected(); }
	if(cur_ == end_ || *cur_++ != 'u') { throw boolean_expected(); }
	if(cur_ == end_ || *cur_++ != 'e') { throw boolean_expected(); }

	return true;
}

//------------------------------------------------------------------------------
// Name: get_null
//------------------------------------------------------------------------------
template <class In>
std::nullptr_t parser<In>::get_null() {
	if(cur_ == end_ || *cur_++ != 'n') { throw keyword_expected(); }
	if(cur_ == end_ || *cur_++ != 'u') { throw keyword_expected(); }
	if(cur_ == end_ || *cur_++ != 'l') { throw keyword_expected(); }
	if(cur_ == end_ || *cur_++ != 'l') { throw keyword_expected(); }

	return nullptr;
}

//------------------------------------------------------------------------------
// Name: get_string
//------------------------------------------------------------------------------
template <class In>
std::string parser<In>::get_string() {

	if(peek() != Quote) {
		throw string_expected();
	}
	++cur_;

	std::string s;
	std::back_insert_iterator<std::string> out = back_inserter(s);

	while(cur_ != end_ && *cur_ != Quote && *cur_ != '\n') {
		if(*cur_ == '\\') {
			++cur_;
			if(cur_ != end_) {
				switch(*cur_) {
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
						if(cur_ == end_) { throw hex_character_expected(); } hex[0] = *++cur_;
						if(cur_ == end_) { throw hex_character_expected(); } hex[1] = *++cur_;
						if(cur_ == end_) { throw hex_character_expected(); } hex[2] = *++cur_;
						if(cur_ == end_) { throw hex_character_expected(); } hex[3] = *++cur_;

						if(!std::isxdigit(hex[0])) throw invalid_unicode_character();
						if(!std::isxdigit(hex[1])) throw invalid_unicode_character();
						if(!std::isxdigit(hex[2])) throw invalid_unicode_character();
						if(!std::isxdigit(hex[3])) throw invalid_unicode_character();

						uint16_t w1 = 0;
						uint16_t w2 = 0;

						w1 |= (detail::to_hex(hex[0]) << 12);
						w1 |= (detail::to_hex(hex[1]) << 8);
						w1 |= (detail::to_hex(hex[2]) << 4);
						w1 |= (detail::to_hex(hex[3]));

						if((w1 & 0xfc00) == 0xdc00) {
							throw invalid_unicode_character();
						}

						if((w1 & 0xfc00) == 0xd800) {
							// part of a surrogate pair
							if(cur_ == end_ || *++cur_ != '\\') { throw utf16_surrogate_expected(); }
							if(cur_ == end_ || *++cur_ != 'u')  { throw utf16_surrogate_expected(); }

							// convert \uXXXX escape sequences to UTF-8
							if(cur_ == end_) { throw hex_character_expected(); } hex[0] = *++cur_;
							if(cur_ == end_) { throw hex_character_expected(); } hex[1] = *++cur_;
							if(cur_ == end_) { throw hex_character_expected(); } hex[2] = *++cur_;
							if(cur_ == end_) { throw hex_character_expected(); } hex[3] = *++cur_;

							if(!std::isxdigit(hex[0])) throw invalid_unicode_character();
							if(!std::isxdigit(hex[1])) throw invalid_unicode_character();
							if(!std::isxdigit(hex[2])) throw invalid_unicode_character();
							if(!std::isxdigit(hex[3])) throw invalid_unicode_character();

							w2 |= (detail::to_hex(hex[0]) << 12);
							w2 |= (detail::to_hex(hex[1]) << 8);
							w2 |= (detail::to_hex(hex[2]) << 4);
							w2 |= (detail::to_hex(hex[3]));
						}

						detail::surrogate_pair_to_utf8(w1, w2, out);
					}
					break;

				default:
					*out++ = '\\';
					break;
				}
			}
		} else {
			*out++ = *cur_;
		}
		++cur_;
	}

	if(*cur_ != Quote || cur_ == end_) {
		throw quote_expected();
	}

	++cur_;

	return s;
}

//------------------------------------------------------------------------------
// Name: get_number
// Desc: retrieves a JSON number. we get it as a string in order to defer
//       conversion to a numeric type until absolutely necessary
//------------------------------------------------------------------------------
template <class In>
std::string parser<In>::get_number() {
	typedef typename std::iterator_traits<In>::iterator_category Cat;
	return get_number(Cat());
}

//------------------------------------------------------------------------------
// Name: get_number
//------------------------------------------------------------------------------
template <class In>
template <class Tr>
std::string parser<In>::get_number(const Tr &) {
	std::string s;
	std::back_insert_iterator<std::string> out = back_inserter(s);

	// JSON numbers fit the regex: -?(0|[1-9][0-9]*)(\.[0-9]+)?([eE][+-]?[0-9]+)?

	// -?
	if(cur_ != end_ && *cur_ == '-') {
		*out++ = *cur_++;
	}

	// (0|[1-9][0-9]*)
	if(cur_ != end_) {
		if(*cur_ >= '1' && *cur_ <= '9') {
			do {
				*out++ = *cur_++;
			} while(cur_ != end_ && std::isdigit(*cur_));
		} else if(*cur_ == '0') {
			*out++ = *cur_++;
		} else {
			throw invalid_number();
		}
	}

	// (\.[0-9]+)?
	if(cur_ != end_ && *cur_ == '.') {
		*out++ = *cur_++;
		if(!std::isdigit(*cur_)) {
			throw invalid_number();
		}

		while(cur_ != end_ && std::isdigit(*cur_)) {
			*out++ = *cur_++;
		}
	}

	// ([eE][+-]?[0-9]+)?
	if(cur_ != end_ && (*cur_ == 'e' || *cur_ == 'E')) {
		*out++ = *cur_++;
		if(cur_ != end_ && (*cur_ == '+' || *cur_ == '-')) {
			*out++ = *cur_++;
		}

		if(!std::isdigit(*cur_)) {
			throw invalid_number();
		}

		while(cur_ != end_ && std::isdigit(*cur_)) {
			*out++ = *cur_++;
		}
	}

	return s;
}

//------------------------------------------------------------------------------
// Name: get_number
// Desc: specialized for random access iterators, so we don't do as much work
//------------------------------------------------------------------------------
template <class In>
std::string parser<In>::get_number(const std::random_access_iterator_tag &) {
	const In first = cur_;

	// -?
	if(cur_ != end_ && *cur_ == '-') {
		++cur_;
	}

	// (0|[1-9][0-9]*)
	if(cur_ != end_) {
		if(*cur_ >= '1' && *cur_ <= '9') {
			do {
				++cur_;
			} while(cur_ != end_ && std::isdigit(*cur_));
		} else if(*cur_ == '0') {
			++cur_;
		} else {
			throw invalid_number();
		}
	}

	// (\.[0-9]+)?
	if(cur_ != end_ && *cur_ == '.') {
		++cur_;
		if(!std::isdigit(*cur_)) {
			throw invalid_number();
		}

		while(cur_ != end_ && std::isdigit(*cur_)) {
			++cur_;
		}
	}

	// ([eE][+-]?[0-9]+)?
	if(cur_ != end_ && (*cur_ == 'e' || *cur_ == 'E')) {
		++cur_;
		if(cur_ != end_ && (*cur_ == '+' || *cur_ == '-')) {
			++cur_;
		}

		if(!std::isdigit(*cur_)) {
			throw invalid_number();
		}

		while(cur_ != end_ && std::isdigit(*cur_)) {
			++cur_;
		}
	}

	return std::string(first, cur_);
}

//------------------------------------------------------------------------------
// Name: get_object
//------------------------------------------------------------------------------
template <class In>
object_pointer parser<In>::get_object() {

	object_pointer obj = std::make_shared<object>();

	if(peek() != ObjectBegin) {
		throw brace_expected();
	}
	++cur_;

	// handle empty object
	char tok = peek();
	if(tok == ObjectEnd) {
		++cur_;
	} else {
		do {
			obj->values_.insert(get_pair());

			tok = peek();
			++cur_;

		} while(tok == ValueSeparator);
	}

	if(tok != ObjectEnd) {
		throw brace_expected();
	}

	return obj;
}

//------------------------------------------------------------------------------
// Name: get_array
//------------------------------------------------------------------------------
template <class In>
array_pointer parser<In>::get_array() {

	array_pointer arr = std::make_shared<array>();

	if(peek() != ArrayBegin) {
		throw bracket_expected();
	}
	++cur_;

	// handle empty object
	char tok = peek();
	if(tok == ArrayEnd) {
		++cur_;
	} else {
		do {
			arr->values_.push_back(get_value());

			tok = peek();
			++cur_;

		} while(tok == ValueSeparator);
	}

	if(tok != ArrayEnd) {
		throw bracket_expected();
	}

	return arr;
}

//------------------------------------------------------------------------------
// Name: get_pair
// Desc: gets a string : value pair (the contents of a JSON object)
//------------------------------------------------------------------------------
template <class In>
std::pair<std::string, value> parser<In>::get_pair() {

	std::string key = get_string();

	if(peek() != NameSeparator) {
		throw colon_expected();
	}
	++cur_;

	return std::make_pair(key, get_value());
}

//------------------------------------------------------------------------------
// Name: get_value
//------------------------------------------------------------------------------
template <class In>
value parser<In>::get_value() {
	switch(peek()) {
	case ObjectBegin:
		return value(get_object());
	case ArrayBegin:
		return value(get_array());
	case Quote:
		return value(get_string());
	case 't':
		return value(get_true());
	case 'f':
		return value(get_false());
	case 'n':
		return value(get_null());
	default:
		return value(get_number(), value::numeric_t());
	}

	throw value_expected();
}

}

#endif
