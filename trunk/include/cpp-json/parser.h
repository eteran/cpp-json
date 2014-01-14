
#ifndef PARSER_20140115_H_
#define PARSER_20140115_H_

namespace json {

class value;

template <class In>
class parser {
public:
	parser(In first, In last);

public:
	value parse();
	
public:
	const In begin() const { return begin_; }
	const In end() const   { return end_; }
	In current() const     { return cur_; }
	
private:
	static const char Quote        = '"';
	static const char LeftBracket  = '[';
	static const char LeftBrace    = '{';
	static const char RightBracket = ']';
	static const char RightBrace   = '}';
	static const char Comma        = ',';
	static const char Colon        = ':';
	
private:
	array_pointer                 get_array();
	bool                          get_false();
	bool                          get_true();
	null_t                        get_null();
	object_pointer                get_object();
	std::pair<std::string, value> get_pair();
	std::string                   get_number();
	std::string                   get_string();
	value                         get_value();

private:
	char peek();
	
private:
	template <class Tr>
	std::string get_number(const Tr &);
	std::string get_number(const std::random_access_iterator_tag &);
	
private:
	const In begin_;
	In       cur_;
	const In end_;
};

}

#endif
