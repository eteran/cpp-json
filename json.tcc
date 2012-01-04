
#ifndef JSON_20110526_TCC_
#define JSON_20110526_TCC_

#include <iterator>
#include <boost/make_shared.hpp>
#include <boost/cstdint.hpp>
#include "json_array.h"

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
			std::cerr << "an error occured " << current - first << " characters into the stream:" << std::endl;
		}
		
		template <class In>
		void print_exception_details(In first, In current, In last) {
			typedef typename std::iterator_traits<In>::iterator_category Cat;
			print_exception_details_internal(first, current, last, Cat());
		}
		
		template <class In>
		json::json_value get_value(In &it, const In &last);
	
		std::vector<uint8_t> ucs2_to_utf8(uint16_t cp);
		
		template <class In>
		typename std::iterator_traits<In>::value_type peek_char(In &it, const In &last);
		
		template <class In>
		json::json_token get_number(In &it, const In &last);
		
		template <class In>
		std::pair<std::string, json::json_value> get_pair(In &it, const In &last);
		
		template <class In>
		json::json_token get_token(In &it, const In &last);
		
		template <class In>
		json::json_token get_string(In &it, const In &last);
		
		template <class In>
		json::json_token get_null(In &it, const In &last);
		
		template <class In>
		json::json_token get_true(In &it, const In &last);
		
		template <class In>
		json::json_token get_false(In &it, const In &last);
	
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
		boost::shared_ptr<json::json_object> get_object(In &it, const In &last) {
			boost::shared_ptr<json_object> object = boost::make_shared<json_object>();

			json_token token = get_token(it, last);
			if(token != json_token("{", json_token::delimeter)) {
				throw brace_expected();
			}

			while(true) {
			
				// handle empty array			
				if(peek_char(it, last) == '}') {
					token = get_token(it, last);
					break;
				}
				
				object->values_.insert(get_pair(it, last));

				token = get_token(it, last);
				if(token != json_token(",", json_token::delimeter)) {
					break;
				}
			}


			if(token != json_token("}", json_token::delimeter)) {
				throw brace_expected();
			}

			return object;
		}

		template <class In>
		boost::shared_ptr<json::json_array> get_array(In &it, const In &last) {
			boost::shared_ptr<json_array> array = boost::make_shared<json_array>();

			json_token token = get_token(it, last);
			if(token != json_token("[", json_token::delimeter)) {
				throw bracket_expected();
			}

			while(true) {

				// handle empty array			
				if(peek_char(it, last) == ']') {
					token = get_token(it, last);
					break;
				}

				array->values_.push_back(get_value(it, last));

				token = get_token(it, last);
				if(token != json_token(",", json_token::delimeter)) {
					break;
				}
			}

			if(token != json_token("]", json_token::delimeter)) {
				throw bracket_expected();
			}

			return array;
		}

		template <class In>
		json::json_value get_value(In &it, const In &last) {
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
		std::pair<std::string, json::json_value> get_pair(In &it, const In &last) {
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
		json::json_token get_token(In &it, const In &last) {
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
		json::json_token get_string(In &it, const In &last) {
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
								if(it == last) { throw invalid_string(); } hex[0] = *++it;
								if(it == last) { throw invalid_string(); } hex[1] = *++it;
								if(it == last) { throw invalid_string(); } hex[2] = *++it;
								if(it == last) { throw invalid_string(); } hex[3] = *++it;
								
								if(!is_hexdigit(hex[0])) throw invalid_unicode_character();
								if(!is_hexdigit(hex[1])) throw invalid_unicode_character();
								if(!is_hexdigit(hex[2])) throw invalid_unicode_character();
								if(!is_hexdigit(hex[3])) throw invalid_unicode_character();
																
								uint16_t ucs2 = 0;
								ucs2 |= ((is_digit(hex[0]) ? hex[0] - '0' : std::toupper(hex[0]) - 'A' + 10) << 12);
								ucs2 |= ((is_digit(hex[1]) ? hex[1] - '0' : std::toupper(hex[1]) - 'A' + 10) << 8);
								ucs2 |= ((is_digit(hex[2]) ? hex[2] - '0' : std::toupper(hex[2]) - 'A' + 10) << 4);
								ucs2 |= ((is_digit(hex[3]) ? hex[3] - '0' : std::toupper(hex[3]) - 'A' + 10));
								
								const std::vector<uint8_t> utf8 = ucs2_to_utf8(ucs2);
								for(std::vector<uint8_t>::const_iterator c_it = utf8.begin(); c_it != utf8.end(); ++c_it) {
									s += *c_it;
								}

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
				throw invalid_string();
			}

			++it;

			return json_token(s, json_token::string);
		}

		template <class In>
		json::json_token get_number(In &it, const In &last) {
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
		json::json_token get_null(In &it, const In &last) {
			
			if(it == last || *it++ != 'n') { throw keyword_expected(); }
			if(it == last || *it++ != 'u') { throw keyword_expected(); }
			if(it == last || *it++ != 'l') { throw keyword_expected(); }
			if(it == last || *it++ != 'l') { throw keyword_expected(); }

			return json_token("null", json_token::null);	
		}
		
		template <class In>
		json::json_token get_true(In &it, const In &last) {
			
			if(it == last || *it++ != 't') { throw boolean_expected(); }
			if(it == last || *it++ != 'r') { throw boolean_expected(); }
			if(it == last || *it++ != 'u') { throw boolean_expected(); }
			if(it == last || *it++ != 'e') { throw boolean_expected(); }

			return json_token("true", json_token::boolean);	
		}
		
		template <class In>
		json::json_token get_false(In &it, const In &last) {
			
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
}

template <class In>
json::value json::parse(In first, In last) {
	const In original_first = first;
	try {
		return detail::get_value(first, last);
	} catch(const json_exception &e) {
		detail::print_exception_details(original_first, first, last);	
		throw;
	}
}

#endif
