
#ifndef JSON_20110526_TCC_
#define JSON_20110526_TCC_

#include <iterator>
#include <boost/make_shared.hpp>
#include <boost/cstdint.hpp>
#include "json_array.h"

template <class In>
json::value json::parse(In first, In last) {
	return detail::get_value(first, last);
}

namespace json {
	namespace detail {
	
	
		template <class Ch>
		bool is_digit(Ch ch) {
			return std::isdigit(ch);
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
		boost::shared_ptr<json::json_value> get_value(In &it, const In &last) {
			switch(peek_char(it, last)) {
			case '{': return boost::make_shared<json_value>(get_object(it, last));
			case '[': return boost::make_shared<json_value>(get_array(it, last));
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
				return boost::make_shared<json_value>(get_number(it, last));
			case '"':
				return boost::make_shared<json_value>(get_string(it, last));
			case 't':
				return boost::make_shared<json_value>(get_true(it, last));
			case 'f':
				return boost::make_shared<json_value>(get_false(it, last));
			case 'n':
				return boost::make_shared<json_value>(get_null(it, last));
			}

			throw value_expected();
		}

		template <class In>
		std::pair<std::string, boost::shared_ptr<json::json_value> > get_pair(In &it, const In &last) {
			json_token token = get_token(it, last);
			if(token.type_ != json_token::string) {
				throw string_expected();
			}

			const std::string key = token.string_;

			token = get_token(it, last);
			if(token != json_token(":", json_token::delimeter)) {
				throw colon_expected();
			}

			boost::shared_ptr<json_value> value = get_value(it, last);

			return std::make_pair(key, value);
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
			// TODO: support \u1234 style escapes
			// TODO: support unicode in general

			std::string s;

			if(*it != '"') {
				throw string_expected();
			}
			++it;

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

#endif
