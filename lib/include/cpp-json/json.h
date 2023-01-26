
#ifndef JSON_20110525_H_
#define JSON_20110525_H_

/* TODO(eteran): support unicode
	00 00 00 xx  UTF-32BE
	00 xx 00 xx  UTF-16BE
	xx 00 00 00  UTF-32LE
	xx 00 xx 00  UTF-16LE
	xx xx xx xx  UTF-8
*/

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdint>
#include <exception>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <variant>
#include <vector>

namespace json {

constexpr int IndentWidth = 4;

class value;
class array;
class object;
class ptr;

using object_entry   = std::pair<std::string, value>;
using object_pointer = std::shared_ptr<object>;
using array_pointer  = std::shared_ptr<array>;

// type testing
inline bool is_string(const value &v) noexcept;
inline bool is_bool(const value &v) noexcept;
inline bool is_number(const value &v) noexcept;
inline bool is_object(const value &v) noexcept;
inline bool is_array(const value &v) noexcept;
inline bool is_null(const value &v) noexcept;

// conversion (you get a copy)
inline std::string to_string(const value &v);
inline bool to_bool(const value &v);
inline object to_object(const value &v);
inline array to_array(const value &v);

template <class T, class = typename std::enable_if<std::is_arithmetic<T>::value>::type>
T to_number(const value &v);

// interpretation (you get a reference)
inline object &as_object(value &v);
inline array &as_array(value &v);
inline std::string &as_string(value &v);
inline const object &as_object(const value &v);
inline const array &as_array(const value &v);
inline const std::string &as_string(const value &v);

// does the given object have a given key?
inline bool has_key(const value &v, std::string_view key) noexcept;
inline bool has_key(const object &o, std::string_view key) noexcept;

// create a value from some JSON
inline value parse(std::string_view s);

// convert a value to a JSON string
enum Options {
	None          = 0x00,
	EscapeUnicode = 0x01,
	PrettyPrint   = 0x02,
};

constexpr inline Options operator&(Options lhs, Options rhs) noexcept {
	using T = std::underlying_type<Options>::type;
	return static_cast<Options>(static_cast<T>(lhs) & static_cast<T>(rhs));
}

constexpr inline Options operator|(Options lhs, Options rhs) noexcept {
	using T = std::underlying_type<Options>::type;
	return static_cast<Options>(static_cast<T>(lhs) | static_cast<T>(rhs));
}

template <class T, class = typename std::enable_if<std::is_same<T, value>::value || std::is_same<T, object>::value || std::is_same<T, array>::value>::type>
std::string stringify(const T &v, Options options = Options::None);

template <class T, class = typename std::enable_if<std::is_same<T, value>::value || std::is_same<T, object>::value || std::is_same<T, array>::value>::type>
void stringify(std::ostream &os, const T &v, Options options = Options::None);

// general error
class exception {
public:
	int line   = -1;
	int column = -1;
};

// parsing errors
class brace_expected : public exception {};
class bracket_expected : public exception {};
class colon_expected : public exception {};
class quote_expected : public exception {};
class invalid_unicode_character : public exception {};
class string_expected : public exception {};
class value_expected : public exception {};
class utf16_surrogate_expected : public exception {};
class invalid_number : public exception {};
class invalid_utf8_string : public exception {};

// usage errors
class invalid_type_cast : public exception {};
class invalid_index : public exception {};

// pointer errors
class invalid_path : public exception {};
class invalid_reference_escape : public exception {};

namespace detail {

/**
 * @brief to_hex
 * @param ch
 * @return
 */
template <class Ch>
unsigned int to_hex(Ch ch) {

	static const unsigned int hex_val[256] = {
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
		0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
		0x0, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
		0x0, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

	if (static_cast<unsigned int>(ch) < 256) {
		return hex_val[static_cast<unsigned int>(ch)];
	} else {
		return 0;
	}
}

/**
 * @brief surrogate_pair_to_utf8
 * @param w1
 * @param w2
 * @param out
 */
template <class Out>
void surrogate_pair_to_utf8(uint16_t w1, uint16_t w2, Out &out) {

	uint32_t cp;
	if ((w1 & 0xfc00) == 0xd800) {
		if ((w2 & 0xfc00) == 0xdc00) {
			cp = 0x10000 + (((static_cast<uint32_t>(w1) & 0x3ff) << 10) | (w2 & 0x3ff));
		} else {
			throw invalid_unicode_character();
		}
	} else {
		cp = w1;
	}

	if (cp < 0x80) {
		*out++ = static_cast<uint8_t>(cp);
	} else if (cp < 0x0800) {
		*out++ = static_cast<uint8_t>(0xc0 | ((cp >> 6) & 0x1f));
		*out++ = static_cast<uint8_t>(0x80 | (cp & 0x3f));
	} else if (cp < 0x10000) {
		*out++ = static_cast<uint8_t>(0xe0 | ((cp >> 12) & 0x0f));
		*out++ = static_cast<uint8_t>(0x80 | ((cp >> 6) & 0x3f));
		*out++ = static_cast<uint8_t>(0x80 | (cp & 0x3f));
	} else if (cp < 0x1fffff) {
		*out++ = static_cast<uint8_t>(0xf0 | ((cp >> 18) & 0x07));
		*out++ = static_cast<uint8_t>(0x80 | ((cp >> 12) & 0x3f));
		*out++ = static_cast<uint8_t>(0x80 | ((cp >> 6) & 0x3f));
		*out++ = static_cast<uint8_t>(0x80 | (cp & 0x3f));
	}
}

}

template <class T, class>
T to_number(const value &v) {
	if (!is_number(v)) {
		throw invalid_type_cast();
	}

	if constexpr (std::is_same_v<T, int64_t>) {
		return stoll(as_string(v), nullptr);
	} else if constexpr (std::is_same_v<T, int32_t>) {
		return static_cast<int32_t>(stol(as_string(v), nullptr));
	} else if constexpr (std::is_same_v<T, int16_t>) {
		return static_cast<int16_t>(stol(as_string(v), nullptr));
	} else if constexpr (std::is_same_v<T, int8_t>) {
		return static_cast<int8_t>(stol(as_string(v), nullptr));
	} else if constexpr (std::is_same_v<T, uint64_t>) {
		return stoull(as_string(v), nullptr);
	} else if constexpr (std::is_same_v<T, uint32_t>) {
		return static_cast<uint32_t>(stoul(as_string(v), nullptr));
	} else if constexpr (std::is_same_v<T, uint16_t>) {
		return static_cast<uint16_t>(stoul(as_string(v), nullptr));
	} else if constexpr (std::is_same_v<T, uint8_t>) {
		return static_cast<uint8_t>(stoul(as_string(v), nullptr));
	} else if constexpr (std::is_same_v<T, double>) {
		return stod(as_string(v), nullptr);
	} else if constexpr (std::is_same_v<T, float>) {
		return stof(as_string(v), nullptr);
	} else {
		throw invalid_type_cast();
	}
}

/**
 * @brief The ptr class
 */
class ptr {
private:
	using C = std::vector<std::string>;

public:
	using allocator_type         = typename C::allocator_type;
	using reference              = typename C::reference;
	using const_reference        = typename C::const_reference;
	using pointer                = typename C::pointer;
	using const_pointer          = typename C::const_pointer;
	using iterator               = typename C::iterator;
	using const_iterator         = typename C::const_iterator;
	using reverse_iterator       = typename C::reverse_iterator;
	using const_reverse_iterator = typename C::const_reverse_iterator;
	using difference_type        = typename C::difference_type;
	using size_type              = typename C::size_type;

public:
	explicit ptr(std::string_view path) {

		auto it = path.begin();

		bool uri_format = false;

		if (it != path.end()) {

			// normal or URI fragment notation?
			if (*it == '#') {
				++it;
				uri_format = true;
			}

			while (it != path.end()) {
				if (*it++ != '/') {
					throw invalid_path();
				}

				std::string reference_token;
				while (it != path.end() && *it != '/') {
					char ch = *it;

					if (!uri_format) {
						if (ch == '~') {

							// ~1 -> /
							// ~0 -> ~

							++it;
							if (it == path.end()) {
								throw invalid_reference_escape();
							}

							switch (*it) {
							case '0':
								ch = '~';
								break;
							case '1':
								ch = '/';
								break;
							default:
								throw invalid_reference_escape();
							}
						}
					} else {
						// %XX -> char(0xXX)

						if (ch == '%') {
							++it;
							if (it == path.end()) {
								throw invalid_reference_escape();
							}

							char hex[2];
							if (!isxdigit(*it)) {
								throw invalid_reference_escape();
							}

							hex[0] = *it++;
							if (it == path.end()) {
								throw invalid_reference_escape();
							}

							if (!isxdigit(*it)) {
								throw invalid_reference_escape();
							}

							hex[1] = *it;

							ch = static_cast<char>((detail::to_hex(hex[0]) << 4) | (detail::to_hex(hex[1])));
						} else if (ch == '~') {
							// ~1 -> /
							// ~0 -> ~

							++it;
							if (it == path.end()) {
								throw invalid_reference_escape();
							}

							switch (*it) {
							case '0':
								ch = '~';
								break;
							case '1':
								ch = '/';
								break;
							default:
								throw invalid_reference_escape();
							}
						}
					}

					reference_token.push_back(ch);
					++it;
				}

				path_.push_back(reference_token);
			}
		}
	}

public:
	ptr()                          = default;
	ptr(ptr &&other)               = default;
	ptr(const ptr &other)          = default;
	ptr &operator=(ptr &&rhs)      = default;
	ptr &operator=(const ptr &rhs) = default;

public:
	iterator begin() noexcept { return path_.begin(); }
	iterator end() noexcept { return path_.end(); }
	const_iterator begin() const noexcept { return path_.begin(); }
	const_iterator end() const noexcept { return path_.end(); }
	const_iterator cbegin() const noexcept { return path_.begin(); }
	const_iterator cend() const noexcept { return path_.end(); }
	reverse_iterator rbegin() noexcept { return path_.rbegin(); }
	reverse_iterator rend() noexcept { return path_.rend(); }
	const_reverse_iterator rbegin() const noexcept { return path_.rbegin(); }
	const_reverse_iterator rend() const noexcept { return path_.rend(); }
	const_reverse_iterator crbegin() const noexcept { return path_.rbegin(); }
	const_reverse_iterator crend() const noexcept { return path_.rend(); }

public:
	size_type size() const noexcept { return path_.size(); }
	size_type max_size() const noexcept { return path_.max_size(); }
	bool empty() const noexcept { return path_.empty(); }

public:
	value operator[](std::size_t n) const;
	value &operator[](std::size_t n);
	value at(std::size_t n) const;
	value &at(std::size_t n);

private:
	C path_;
};

/**
 * @brief The object class
 */
class object {
	friend bool operator==(const object &lhs, const object &rhs) noexcept;
	friend bool operator!=(const object &lhs, const object &rhs) noexcept;

	friend class parser;

private:
	using C = std::vector<object_entry>;

public:
	using allocator_type  = typename C::allocator_type;
	using reference       = typename C::reference;
	using const_reference = typename C::const_reference;
	using pointer         = typename C::pointer;
	using const_pointer   = typename C::const_pointer;
	using iterator        = typename C::iterator;
	using const_iterator  = typename C::const_iterator;
	using difference_type = typename C::difference_type;
	using size_type       = typename C::size_type;

public:
	object()                             = default;
	object(const object &other)          = default;
	object(object &&other)               = default;
	object &operator=(const object &rhs) = default;
	object &operator=(object &&rhs)      = default;
	object(std::initializer_list<object_entry> list);

public:
	iterator begin() noexcept { return values_.begin(); }
	iterator end() noexcept { return values_.end(); }
	const_iterator begin() const noexcept { return values_.begin(); }
	const_iterator end() const noexcept { return values_.end(); }
	const_iterator cbegin() const noexcept { return values_.begin(); }
	const_iterator cend() const noexcept { return values_.end(); }

public:
	iterator find(std::string_view s) noexcept;
	const_iterator find(std::string_view s) const noexcept;

public:
	size_type size() const noexcept {
		return values_.size();
	}

	size_type max_size() const noexcept {
		return values_.max_size();
	}

	bool empty() const noexcept {
		return values_.empty();
	}

public:
	value operator[](std::string_view key) const;
	value &operator[](std::string_view key);

	value at(std::string_view key) const;
	value &at(std::string_view key);

public:
	template <class T>
	std::pair<iterator, bool> insert(std::string key, const T &v);

	template <class T>
	std::pair<iterator, bool> insert(std::string key, T &&v);

	template <class T>
	std::pair<iterator, bool> insert(std::pair<std::string, T> &&p);

public:
	void swap(object &other) noexcept;

private:
	C values_;

	// NOTE(eteran): The values are stored in insertion order above,
	// but we use this map to have a fast lookup of key -> index
	std::map<std::string, size_t, std::less<>> index_map_;
};

inline object::iterator begin(object &obj) noexcept {
	return obj.begin();
}

inline object::iterator end(object &obj) noexcept {
	return obj.end();
}

inline object::const_iterator begin(const object &obj) noexcept {
	return obj.begin();
}

inline object::const_iterator end(const object &obj) noexcept {
	return obj.end();
}

inline object::const_iterator cbegin(const object &obj) noexcept {
	return obj.begin();
}

inline object::const_iterator cend(const object &obj) noexcept {
	return obj.end();
}

/**
 * @brief The array class
 */
class array {
	friend bool operator==(const array &lhs, const array &rhs) noexcept;
	friend bool operator!=(const array &lhs, const array &rhs) noexcept;

private:
	using C = std::vector<value>;

public:
	using allocator_type         = typename C::allocator_type;
	using reference              = typename C::reference;
	using const_reference        = typename C::const_reference;
	using pointer                = typename C::pointer;
	using const_pointer          = typename C::const_pointer;
	using iterator               = typename C::iterator;
	using const_iterator         = typename C::const_iterator;
	using reverse_iterator       = typename C::reverse_iterator;
	using const_reverse_iterator = typename C::const_reverse_iterator;
	using difference_type        = typename C::difference_type;
	using size_type              = typename C::size_type;

public:
	array()                            = default;
	array(array &&other)               = default;
	array(const array &other)          = default;
	array &operator=(array &&rhs)      = default;
	array &operator=(const array &rhs) = default;
	array(std::initializer_list<value> list);

	template <class In>
	array(In first, In last) {
		values_.insert(values_.end(), first, last);
	}

public:
	iterator begin() noexcept { return values_.begin(); }
	iterator end() noexcept { return values_.end(); }
	const_iterator begin() const noexcept { return values_.begin(); }
	const_iterator end() const noexcept { return values_.end(); }
	const_iterator cbegin() const noexcept { return values_.begin(); }
	const_iterator cend() const noexcept { return values_.end(); }
	reverse_iterator rbegin() noexcept { return values_.rbegin(); }
	reverse_iterator rend() noexcept { return values_.rend(); }
	const_reverse_iterator rbegin() const noexcept { return values_.rbegin(); }
	const_reverse_iterator rend() const noexcept { return values_.rend(); }
	const_reverse_iterator crbegin() const noexcept { return values_.rbegin(); }
	const_reverse_iterator crend() const noexcept { return values_.rend(); }

public:
	size_type size() const noexcept { return values_.size(); }
	size_type max_size() const noexcept { return values_.max_size(); }
	bool empty() const noexcept { return values_.empty(); }

public:
	value operator[](std::size_t n) const;
	value &operator[](std::size_t n);
	value at(std::size_t n) const;
	value &at(std::size_t n);

public:
	template <class T>
	void push_back(T &&v) {
		values_.emplace_back(std::forward<T>(v));
	}

	template <class T>
	void push_back(const T &v) {
		values_.emplace_back(v);
	}

	void pop_back() noexcept {
		values_.pop_back();
	}

public:
	void swap(array &other) noexcept {
		using std::swap;
		swap(values_, other.values_);
	}

private:
	C values_;
};

inline array::iterator begin(array &arr) noexcept {
	return arr.begin();
}

inline array::iterator end(array &arr) noexcept {
	return arr.end();
}

inline array::const_iterator begin(const array &arr) noexcept {
	return arr.begin();
}

inline array::const_iterator end(const array &arr) noexcept {
	return arr.end();
}

inline array::const_iterator cbegin(const array &arr) noexcept {
	return arr.begin();
}

inline array::const_iterator cend(const array &arr) noexcept {
	return arr.end();
}

inline array::reverse_iterator rbegin(array &arr) noexcept {
	return arr.rbegin();
}

inline array::reverse_iterator rend(array &arr) noexcept {
	return arr.rend();
}

inline array::const_reverse_iterator rbegin(const array &arr) noexcept {
	return arr.rbegin();
}

inline array::const_reverse_iterator rend(const array &arr) noexcept {
	return arr.rend();
}

inline array::const_reverse_iterator crbegin(const array &arr) noexcept {
	return arr.rbegin();
}

inline array::const_reverse_iterator crend(const array &arr) noexcept {
	return arr.rend();
}

/**
 * @brief The value class
 */
class value {
	friend bool to_bool(const value &v);

	friend bool operator==(const value &lhs, const value &rhs);
	friend bool operator!=(const value &lhs, const value &rhs);

	friend class parser;

private:
	struct numeric_type {};
	// create a value from a numeric string, internal use only!
	value(std::string s, const numeric_type &)
		: storage_(std::move(s)), type_(type_number) {
	}

public:
	// initialize from basic types
	explicit value(const array &a);
	explicit value(const object &o);

	value(array &&a);
	value(object &&o);

	value(bool b)
		: storage_(b ? Boolean::True : Boolean::False), type_(type_boolean) {
	}

	// NOTE(eteran): we don't use string_view here because of the bool overload
	// which necessitates that we have a const char * overload to prevent value("hello")
	// from creating a "True" value. Since we need this overload anyway, no real benefit
	// to using a string_view
	value(const char *s)
		: storage_(std::string(s)), type_(type_string) {
	}

	value(std::string s)
		: storage_(std::move(s)), type_(type_string) {
	}

	template <class T, typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
	value(T n)
		: storage_(std::to_string(n)), type_(type_number) {
	}

	value(const std::nullptr_t &)
		: storage_(Null()), type_(type_null) {
	}

	value()
		: storage_(Null()), type_(type_null) {
	}

public:
	~value() = default;

private:
	explicit value(object_pointer o);
	explicit value(array_pointer a);

public:
	value(const value &other)
		: storage_(other.storage_), type_(other.type_) {
	}

	value(value &&other)
		: storage_(std::move(other.storage_)), type_(other.type_) {
	}

public:
	value &operator=(const value &rhs);
	value &operator=(value &&rhs);

public:
	void swap(value &other) noexcept {
		using std::swap;
		swap(storage_, other.storage_);
		swap(type_, other.type_);
	}

public:
	enum Type {
		type_null,
		type_boolean,
		type_object,
		type_array,
		type_string,
		type_number,
	};

	Type type() const noexcept { return type_; }

public:
	value operator[](std::string_view key) const;
	value operator[](std::size_t n) const;
	value &operator[](std::string_view key);
	value &operator[](std::size_t n);

public:
	inline value at(std::size_t n) const;
	inline value &at(std::size_t n);
	inline value at(std::string_view key) const;
	inline value &at(std::string_view key);

public:
	value operator[](const ptr &ptr) const;
	value &operator[](const ptr &ptr);

	value &create(const ptr &ptr);

public:
	// array like interface
	template <class T>
	void push_back(T &&v);

	template <class T>
	void push_back(const T &v);

public:
	// object like interface
	template <class T>
	std::pair<object::iterator, bool> insert(std::string key, const T &v);

	template <class T>
	std::pair<object::iterator, bool> insert(std::string key, T &&v);

	template <class T>
	std::pair<object::iterator, bool> insert(std::pair<std::string, T> &&p);

public:
	// object/array like
	size_t size() const {
		if (is_object()) {
			return as_object().size();
		} else if (is_array()) {
			return as_array().size();
		}

		throw invalid_type_cast();
	}

public:
	bool is_string() const noexcept {
		return (type_ == value::type_string);
	}

	bool is_bool() const noexcept {
		return (type_ == value::type_boolean);
	}

	bool is_number() const noexcept {
		return (type_ == value::type_number);
	}

	bool is_object() const noexcept {
		return (type_ == value::type_object);
	}

	bool is_array() const noexcept {
		return (type_ == value::type_array);
	}

	bool is_null() const noexcept {
		return (type_ == value::type_null);
	}

public:
	const std::string &as_string() const {

		switch (type_) {
		case value::type_string:
		case value::type_number:
			return std::get<std::string>(storage_);
		default:
			throw invalid_type_cast();
		}
	}

	std::string &as_string() {
		switch (type_) {
		case value::type_string:
		case value::type_number:
			return std::get<std::string>(storage_);
		default:
			throw invalid_type_cast();
		}
	}

	const object &as_object() const {
		if (type_ != type_object) {
			throw invalid_type_cast();
		}

		return *std::get<object_pointer>(storage_);
	}

	object &as_object() {
		if (type_ != type_object) {
			throw invalid_type_cast();
		}

		return *std::get<object_pointer>(storage_);
	}

	const array &as_array() const {
		if (type_ != type_array) {
			throw invalid_type_cast();
		}

		return *std::get<array_pointer>(storage_);
	}

	array &as_array() {
		if (type_ != type_array) {
			throw invalid_type_cast();
		}

		return *std::get<array_pointer>(storage_);
	}

private:
	struct Null {};

	enum class Boolean {
		False,
		True,
	};

	std::variant<Null, Boolean, object_pointer, array_pointer, std::string> storage_;
	Type type_;
};

inline value array::operator[](std::size_t n) const {
	return at(n);
}

inline value &array::operator[](std::size_t n) {
	return at(n);
}

inline value array::at(std::size_t n) const {
	if (n < values_.size()) {
		return values_[n];
	}

	throw invalid_index();
}

inline value &array::at(std::size_t n) {
	if (n < values_.size()) {
		return values_[n];
	}

	throw invalid_index();
}

struct location_type {
	size_t line;
	size_t column;
};

class reader {
	friend class parser;

public:
	reader(std::string_view s) noexcept
		: str_(s) {
	}

public:
	template <class T>
	void require(char ch) {
		if (!match(ch)) {
			throw T();
		}
	}

	bool match(char ch) noexcept {
		if (peek() != ch) {
			return false;
		}

		++index_;
		return true;
	}

	bool match(std::string_view s) noexcept {

		if (str_.compare(index_, s.size(), s) != 0) {
			return false;
		}

		index_ += s.size();
		return true;
	}

public:
	location_type location() noexcept {
		return location(index_);
	}

	location_type location(size_t index) noexcept {
		size_t line = 1;
		size_t col  = 1;

		if (index < str_.size()) {

			for (size_t i = 0; i < index; ++i) {
				if (str_[i] == '\n') {
					++line;
					col = 1;
				} else {
					++col;
				}
			}
		}

		return location_type{line, col};
	}

public:
	char peek() noexcept {
		if (at_end()) {
			return '\0';
		}

		return str_[index_];
	}

	char read() noexcept {
		if (at_end()) {
			return '\0';
		}

		return str_[index_++];
	}

	bool at_end() const noexcept {
		return index_ == str_.size();
	}

private:
	size_t index_ = 0;
	std::string_view str_;
};

/**
 * @brief The parser class
 */
class parser {
public:
	enum token_type {
		ArrayBegin,
		ArrayEnd,
		Colon,
		Comma,
		ObjectBegin,
		ObjectEnd,
		Quote,
		Number,
		String,
		LiteralTrue,
		LiteralFalse,
		LiteralNull,
	};

	struct token {
		token_type type;

		// TODO(eteran): make this a string view, and reduce copies?
		std::string value;
		size_t index;
	};

private:
	template <class T>
	token require(token_type type) {
		if (auto tok = match(type)) {
			return *tok;
		}

		throw T();
	}

	std::optional<token> match(token_type type) noexcept {
		if (tokens_[index_].type == type) {
			return tokens_[index_++];
		}
		return {};
	}

private:
	std::optional<token> next_token() {
		// consume whitespace
		while (std::isspace(reader_.peek())) {
			reader_.read();
		}

		// if we reach end of the stream, we're done!
		if (reader_.at_end()) {
			return {};
		}

		const size_t token_start = reader_.index_;

		// extract the next token...
		if (reader_.match('[')) {
			return token{token_type::ArrayBegin, "[", token_start};
		} else if (reader_.match(']')) {
			return token{token_type::ArrayEnd, "]", token_start};
		} else if (reader_.match(':')) {
			return token{token_type::Colon, ":", token_start};
		} else if (reader_.match(',')) {
			return token{token_type::Comma, ",", token_start};
		} else if (reader_.match('{')) {
			return token{token_type::ObjectBegin, "{", token_start};
		} else if (reader_.match('}')) {
			return token{token_type::ObjectEnd, "}", token_start};
		} else if (reader_.match("true")) {
			return token{token_type::LiteralTrue, "true", token_start};
		} else if (reader_.match("false")) {
			return token{token_type::LiteralFalse, "false", token_start};
		} else if (reader_.match("null")) {
			return token{token_type::LiteralNull, "null", token_start};
		} else if (reader_.match('"')) {

			std::string s;
			std::back_insert_iterator<std::string> out = back_inserter(s);

			while (reader_.peek() != '"' && reader_.peek() != '\n') {

				char ch = reader_.read();
				if (ch == '\\') {
					switch (reader_.read()) {
					case '"':
						*out++ = '"';
						break;
					case '\\':
						*out++ = '\\';
						break;
					case '/':
						*out++ = '/';
						break;
					case 'b':
						*out++ = '\b';
						break;
					case 'f':
						*out++ = '\f';
						break;
					case 'n':
						*out++ = '\n';
						break;
					case 'r':
						*out++ = '\r';
						break;
					case 't':
						*out++ = '\t';
						break;
					case 'u': {
						// convert \uXXXX escape sequences to UTF-8
						char hex[4];

						if (!std::isxdigit(hex[0] = reader_.read())) throw invalid_unicode_character();
						if (!std::isxdigit(hex[1] = reader_.read())) throw invalid_unicode_character();
						if (!std::isxdigit(hex[2] = reader_.read())) throw invalid_unicode_character();
						if (!std::isxdigit(hex[3] = reader_.read())) throw invalid_unicode_character();

						uint16_t w1 = 0;
						uint16_t w2 = 0;

						w1 |= (detail::to_hex(hex[0]) << 12);
						w1 |= (detail::to_hex(hex[1]) << 8);
						w1 |= (detail::to_hex(hex[2]) << 4);
						w1 |= (detail::to_hex(hex[3]));

						if ((w1 & 0xfc00) == 0xdc00) {
							throw invalid_unicode_character();
						}

						if ((w1 & 0xfc00) == 0xd800) {
							// part of a surrogate pair
							if (!reader_.match("\\u")) {
								throw utf16_surrogate_expected();
							}

							// convert \uXXXX escape sequences for surrogate pairs to UTF-8
							if (!std::isxdigit(hex[0] = reader_.read())) throw invalid_unicode_character();
							if (!std::isxdigit(hex[1] = reader_.read())) throw invalid_unicode_character();
							if (!std::isxdigit(hex[2] = reader_.read())) throw invalid_unicode_character();
							if (!std::isxdigit(hex[3] = reader_.read())) throw invalid_unicode_character();

							w2 |= (detail::to_hex(hex[0]) << 12);
							w2 |= (detail::to_hex(hex[1]) << 8);
							w2 |= (detail::to_hex(hex[2]) << 4);
							w2 |= (detail::to_hex(hex[3]));
						}

						detail::surrogate_pair_to_utf8(w1, w2, out);
						break;
					}
					default:
						*out++ = '\\';
						break;
					}
				} else {
					*out++ = ch;
				}
			}

			reader_.require<quote_expected>('"');
			return token{token_type::String, s, token_start};

		} else {

			std::string s;
			s.reserve(10);
			std::back_insert_iterator<std::string> out = back_inserter(s);

			// JSON numbers fit the regex: -?(0|[1-9][0-9]*)(\.[0-9]+)?([eE][+-]?[0-9]+)?

			// -?
			if (reader_.match('-')) {
				*out++ = '-';
			}

			// (0|[1-9][0-9]*)
			if (reader_.match('0')) {
				*out++ = '0';
			} else {
				if (!std::isdigit(reader_.peek())) {
					throw invalid_number();
				}

				while (std::isdigit(reader_.peek())) {
					*out++ = reader_.read();
				}
			}

			// (\.[0-9]+)?
			if (reader_.match('.')) {
				*out++ = '.';
				if (!std::isdigit(reader_.peek())) {
					throw invalid_number();
				}

				while (std::isdigit(reader_.peek())) {
					*out++ = reader_.read();
				}
			}

			// ([eE][+-]?[0-9]+)?
			if (reader_.peek() == 'e' || reader_.peek() == 'E') {
				*out++ = reader_.read();
				if (reader_.peek() == '+' || reader_.peek() == '-') {
					*out++ = reader_.read();
				}

				if (!std::isdigit(reader_.peek())) {
					throw invalid_number();
				}

				while (std::isdigit(reader_.peek())) {
					*out++ = reader_.read();
				}
			}

			return token{token_type::Number, s, token_start};
		}
	}

	std::vector<token> tokenize() {
		std::vector<token> tokens;

		while (auto tok = next_token()) {
			tokens.push_back(*tok);
		}

		return tokens;
	}

public:
	parser(std::string_view s)
		: reader_(s) {

		tokens_ = tokenize();
	}

public:
	value parse() {
		return get_value();
	}

private:
	array_pointer get_array() {

		auto arr = std::make_shared<array>();

		if (!match(token_type::ArrayEnd)) {
			do {
				arr->push_back(get_value());
			} while (match(token_type::Comma));
		}

		require<bracket_expected>(token_type::ArrayEnd);
		return arr;
	}

	object_pointer get_object() {

		auto obj = std::make_shared<object>();

		if (!match(token_type::ObjectEnd)) {
			do {
				const token &key = require<string_expected>(token_type::String);
				require<colon_expected>(token_type::Colon);
				obj->insert(key.value, get_value());
			} while (match(token_type::Comma));
		}

		require<brace_expected>(token_type::ObjectEnd);
		return obj;
	}

	value get_value() {

		if (match(token_type::ObjectBegin)) {
			return value(get_object());
		} else if (match(token_type::ArrayBegin)) {
			return value(get_array());
		} else if (match(token_type::LiteralTrue)) {
			return value(true);
		} else if (match(token_type::LiteralFalse)) {
			return value(false);
		} else if (match(token_type::LiteralNull)) {
			return value(nullptr);
		} else if (auto str_token = match(token_type::String)) {
			return value(str_token->value);
		} else if (auto num_token = match(token_type::Number)) {
			return value(num_token->value, value::numeric_type());
		} else {
			throw value_expected();
		}
	}

public:
	location_type location() noexcept {
		return reader_.location(tokens_[index_].index);
	}

private:
	reader reader_;
	std::vector<token> tokens_;
	size_t index_ = 0;
};

inline std::string to_string(const value &v) {
	return as_string(v);
}

inline bool to_bool(const value &v) {
	if (!is_bool(v)) {
		throw invalid_type_cast();
	}

	return std::get<value::Boolean>(v.storage_) == value::Boolean::True;
}

inline object to_object(const value &v) {
	return as_object(v);
}

inline array to_array(const value &v) {
	return as_array(v);
}

inline object &as_object(array &v) {
	(void)v;
	throw invalid_type_cast();
}

inline array &as_array(object &v) {
	(void)v;
	throw invalid_type_cast();
}

inline const object &as_object(const array &v) {
	(void)v;
	throw invalid_type_cast();
}

inline const array &as_array(const object &v) {
	(void)v;
	throw invalid_type_cast();
}

inline object &as_object(value &v) {
	if (!is_object(v)) {
		throw invalid_type_cast();
	}

	return v.as_object();
}

inline const object &as_object(const value &v) {
	if (!is_object(v)) {
		throw invalid_type_cast();
	}

	return v.as_object();
}

inline array &as_array(value &v) {
	if (!is_array(v)) {
		throw invalid_type_cast();
	}

	return v.as_array();
}

inline const array &as_array(const value &v) {
	if (!is_array(v)) {
		throw invalid_type_cast();
	}

	return v.as_array();
}

const std::string &as_string(const value &v) {
	if (!is_string(v) && !is_number(v)) {
		throw invalid_type_cast();
	}

	return v.as_string();
}

std::string &as_string(value &v) {
	if (!is_string(v) && !is_number(v)) {
		throw invalid_type_cast();
	}

	return v.as_string();
}

inline bool has_key(const value &v, std::string_view key) noexcept {
	if (is_object(v)) {
		return has_key(as_object(v), key);
	}
	return false;
}

inline bool has_key(const object &o, std::string_view key) noexcept {
	return o.find(key) != o.end();
}

inline value parse(std::string_view s) {
	parser p(s);

	try {
		return p.parse();
	} catch (exception &e) {
		auto loc = p.location();
		e.line   = loc.line;
		e.column = loc.column;
		throw;
	}
}

inline bool is_string(const value &v) noexcept {
	return v.is_string();
}

inline bool is_bool(const value &v) noexcept {
	return v.is_bool();
}

inline bool is_number(const value &v) noexcept {
	return v.is_number();
}

inline bool is_object(const value &v) noexcept {
	return v.is_object();
}

inline bool is_array(const value &v) noexcept {
	return v.is_array();
}

inline bool is_null(const value &v) noexcept {
	return v.is_null();
}

namespace detail {

inline std::string escape_string(std::string_view s, Options options) {

	std::string r;
	r.reserve(s.size());

	if (options & Options::EscapeUnicode) {
		struct state_t {
			unsigned int
				expected : 4,
				seen : 4,
				reserved : 24;
		};

		state_t shift_state = {0, 0, 0};
		char32_t result     = 0;

		for (auto it = s.begin(); it != s.end(); ++it) {

			const auto ch = static_cast<uint8_t>(*it);

			if (shift_state.seen == 0) {

				if ((ch & 0x80) == 0) {
					switch (ch) {
					case '\"':
						r += "\\\"";
						break;
					case '\\':
						r += "\\\\";
						break;
#if 0
					case '/':  r += "\\/"; break;
#endif
					case '\b':
						r += "\\b";
						break;
					case '\f':
						r += "\\f";
						break;
					case '\n':
						r += "\\n";
						break;
					case '\r':
						r += "\\r";
						break;
					case '\t':
						r += "\\t";
						break;
					default:
						if (!isprint(ch)) {
							r += "\\u";
							char buf[5];
							snprintf(buf, sizeof(buf), "%04X", ch);
							r += buf;
						} else {
							r += static_cast<char>(ch);
						}
						break;
					}
				} else if ((ch & 0xe0) == 0xc0) {
					// 2 byte
					result               = ch & 0x1f;
					shift_state.expected = 2;
					shift_state.seen     = 1;
				} else if ((ch & 0xf0) == 0xe0) {
					// 3 byte
					result               = ch & 0x0f;
					shift_state.expected = 3;
					shift_state.seen     = 1;
				} else if ((ch & 0xf8) == 0xf0) {
					// 4 byte
					result               = ch & 0x07;
					shift_state.expected = 4;
					shift_state.seen     = 1;
				} else if ((ch & 0xfc) == 0xf8) {
					// 5 byte
					throw invalid_utf8_string(); // Restricted by RFC 3629
				} else if ((ch & 0xfe) == 0xfc) {
					// 6 byte
					throw invalid_utf8_string(); // Restricted by RFC 3629
				} else {
					throw invalid_utf8_string(); // should never happen
				}
			} else if (shift_state.seen < shift_state.expected) {
				if ((ch & 0xc0) == 0x80) {
					result <<= 6;
					result |= ch & 0x3f;
					// increment the shift state
					++shift_state.seen;

					if (shift_state.seen == shift_state.expected) {
						// done with this character

						char buf[5];

						if (result < 0xd800 || (result >= 0xe000 && result < 0x10000)) {
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
						result               = 0;
					}

				} else {
					throw invalid_utf8_string(); // should never happen
				}
			} else {
				throw invalid_utf8_string(); // should never happen
			}
		}
	} else {

		for (char ch : s) {

			switch (ch) {
			case '\"':
				r += "\\\"";
				break;
			case '\\':
				r += "\\\\";
				break;
#if 0
			case '/':  r += "\\/"; break;
#endif
			case '\b':
				r += "\\b";
				break;
			case '\f':
				r += "\\f";
				break;
			case '\n':
				r += "\\n";
				break;
			case '\r':
				r += "\\r";
				break;
			case '\t':
				r += "\\t";
				break;
			default:
				r += ch;
				break;
			}
		}
	}
	return r;
}

inline std::string escape_string(std::string_view s) {
	return escape_string(s, Options::None);
}

// pretty print as a string
inline void value_to_string(std::ostream &os, const value &v, Options options, int indent, bool ignore_initial_ident);

inline void value_to_string(std::ostream &os, const object &o, Options options, int indent, bool ignore_initial_ident) {

	if (!ignore_initial_ident) {
		os << std::string(indent * IndentWidth, ' ');
	}

	if (o.empty()) {
		os << "{}";
	} else {
		os << "{\n";

		auto it = o.begin();
		auto e  = o.end();

		++indent;
		os << std::string(indent * IndentWidth, ' ') << '"' << escape_string(it->first, options) << "\" : ";
		value_to_string(os, it->second, options, indent, true);

		++it;
		for (; it != e; ++it) {
			os << ',';
			os << '\n';
			os << std::string(indent * IndentWidth, ' ') << '"' << escape_string(it->first, options) << "\" : ";
			value_to_string(os, it->second, options, indent, true);
		}
		--indent;

		os << "\n";
		os << std::string(indent * IndentWidth, ' ') << "}";
	}
}

inline void value_to_string(std::ostream &os, const array &a, Options options, int indent, bool ignore_initial_ident) {

	if (!ignore_initial_ident) {
		os << std::string(indent * IndentWidth, ' ');
	}

	if (a.empty()) {
		os << "[]";
	} else {
		os << "[\n";

		auto it = a.begin();
		auto e  = a.end();

		++indent;
		value_to_string(os, *it++, options, indent, false);

		for (; it != e; ++it) {
			os << ',';
			os << '\n';
			value_to_string(os, *it, options, indent, false);
		}
		--indent;

		os << "\n";
		os << std::string(indent * IndentWidth, ' ') << "]";
	}
}

inline void value_to_string(std::ostream &os, const value &v, Options options, int indent, bool ignore_initial_ident) {

	if (!ignore_initial_ident) {
		os << std::string(indent * IndentWidth, ' ');
	}

	switch (v.type()) {
	case value::type_string:
		os << '"' << escape_string(as_string(v), options) << '"';
		break;
	case value::type_number:
		os << as_string(v);
		break;
	case value::type_null:
		os << "null";
		break;
	case value::type_boolean:
		os << (to_bool(v) ? "true" : "false");
		break;
	case value::type_object:
		value_to_string(os, as_object(v), options, indent, true);
		break;
	case value::type_array:
		value_to_string(os, as_array(v), options, indent, true);
		break;
	}
}

inline std::string value_to_string(const value &v, Options options, int indent, bool ignore_initial_ident) {

	std::stringstream ss;
	value_to_string(ss, v, options, indent, ignore_initial_ident);
	return ss.str();
}

inline std::string value_to_string(const value &v, Options options) {
	return value_to_string(v, options, 0, false);
}

inline void value_to_string(std::ostream &os, const value &v, Options options) {
	value_to_string(os, v, options, 0, false);
}

// serialize, not pretty printed
inline void serialize(std::ostream &os, const value &v, Options options);

inline void serialize(std::ostream &os, const array &a, Options options) {
	os << "[";
	if (!a.empty()) {
		auto it = a.begin();
		auto e  = a.end();

		serialize(os, *it++, options);

		for (; it != e; ++it) {
			os << ',';
			serialize(os, *it, options);
		}
	}
	os << "]";
}

inline void serialize(std::ostream &os, const object &o, Options options) {
	os << "{";
	if (!o.empty()) {
		auto it = o.begin();
		auto e  = o.end();

		os << '"' << escape_string(it->first, options) << "\":";
		serialize(os, it->second, options);
		++it;
		for (; it != e; ++it) {
			os << ',';
			os << '"' << escape_string(it->first, options) << "\":";
			serialize(os, it->second, options);
		}
	}
	os << "}";
}

inline void serialize(std::ostream &os, const value &v, Options options) {

	switch (v.type()) {
	case value::type_string:
		os << '"' << escape_string(as_string(v), options) << '"';
		break;
	case value::type_number:
		os << as_string(v);
		break;
	case value::type_null:
		os << "null";
		break;
	case value::type_boolean:
		os << (to_bool(v) ? "true" : "false");
		break;
	case value::type_object:
		serialize(os, as_object(v), options);
		break;
	case value::type_array:
		serialize(os, as_array(v), options);
		break;
	}
}

template <class T, class = typename std::enable_if<std::is_same<T, value>::value || std::is_same<T, object>::value || std::is_same<T, array>::value>::type>
std::string serialize(const T &v, Options options) {
	std::stringstream ss;

	std::locale c_locale("C");
	ss.imbue(c_locale);

	serialize(ss, v, options);
	return ss.str();
}

template <class T, class = typename std::enable_if<std::is_same<T, value>::value || std::is_same<T, object>::value || std::is_same<T, array>::value>::type>
std::string pretty_print(const T &v, Options options) {
	return value_to_string(value(v), options);
}

template <class T, class = typename std::enable_if<std::is_same<T, value>::value || std::is_same<T, object>::value || std::is_same<T, array>::value>::type>
void pretty_print(std::ostream &os, const T &v, Options options) {
	value_to_string(os, value(v), options);
}

}

template <class T, class>
std::string stringify(const T &v, Options options) {
	if (options & Options::PrettyPrint) {
		return detail::pretty_print(v, options);
	} else {
		return detail::serialize(v, options);
	}
}

template <class T, class>
void stringify(std::ostream &os, const T &v, Options options) {

	std::locale c_locale("C");
	os.imbue(c_locale);

	if (options & Options::PrettyPrint) {
		detail::pretty_print(os, v, options);
	} else {
		detail::serialize(os, v, options);
	}
}

/**
 * @brief object::swap
 * @param other
 */
inline void object::swap(object &other) noexcept {
	using std::swap;
	swap(values_, other.values_);
	swap(index_map_, other.index_map_);
}

/**
 * @brief object::object
 * @param list
 */
inline object::object(std::initializer_list<object_entry> list) {

	for (auto &entry : list) {
		insert(entry.first, entry.second);
	}
}

inline value object::operator[](std::string_view key) const {
	return at(key);
}

inline value &object::operator[](std::string_view key) {
	return at(key);
}

inline object::iterator object::find(std::string_view s) noexcept {

	auto it = index_map_.find(s);
	if (it != index_map_.end()) {
		return values_.begin() + it->second;
	}

	return values_.end();
}

inline object::const_iterator object::find(std::string_view s) const noexcept {
	auto it = index_map_.find(s);
	if (it != index_map_.end()) {
		return values_.begin() + it->second;
	}

	return values_.end();
}

/**
 * @brief object::at
 * @param key
 * @return
 */
inline value object::at(std::string_view key) const {

	auto it = index_map_.find(key);
	if (it != index_map_.end()) {
		return values_[it->second].second;
	}

	throw invalid_index();
}

/**
 * @brief object::at
 * @param key
 * @return
 */
inline value &object::at(std::string_view key) {

	auto it = index_map_.find(key);
	if (it != index_map_.end()) {
		return values_[it->second].second;
	}

	throw invalid_index();
}

/**
 * @brief object::insert
 * @param p
 * @return
 */
template <class T>
auto object::insert(std::pair<std::string, T> &&p) -> std::pair<iterator, bool> {
	return insert(std::move(p.first), std::move(p.second));
}

/**
 * @brief object::insert
 * @param key
 * @param v
 * @return
 */
template <class T>
auto object::insert(std::string key, const T &v) -> std::pair<iterator, bool> {

	auto it = find(key);
	if (it != values_.end()) {
		return std::make_pair(it, false);
	}

	auto n = values_.emplace(it, std::move(key), value(v));
	index_map_.emplace(n->first, values_.size() - 1);
	return std::make_pair(n, true);
}

/**
 * @brief object::insert
 * @param key
 * @param v
 * @return
 */
template <class T>
auto object::insert(std::string key, T &&v) -> std::pair<iterator, bool> {

	auto it = find(key);
	if (it != values_.end()) {
		return std::make_pair(it, false);
	}

	auto n = values_.emplace(it, std::move(key), value(std::forward<T>(v)));
	index_map_.emplace(n->first, values_.size() - 1);
	return std::make_pair(n, true);
}

/**
 * @brief array::array
 * @param list
 */
inline array::array(std::initializer_list<value> list) {
	for (const auto &x : list) {
		values_.emplace_back(x);
	}
}

/**
 * @brief value::value
 * @param o
 */
inline value::value(object_pointer o)
	: storage_(std::move(o)), type_(type_object) {
}

/**
 * @brief value::value
 * @param a
 */
inline value::value(array_pointer a)
	: storage_(std::move(a)), type_(type_array) {
}

/**
 * @brief value::operator =
 * @param rhs
 * @return
 */
inline value &value::operator=(value &&rhs) {
	if (this != &rhs) {
		storage_ = std::move(rhs.storage_);
		type_    = std::move(rhs.type_);
	}

	return *this;
}

/**
 * @brief value::operator =
 * @param rhs
 * @return
 */
inline value &value::operator=(const value &rhs) {

	if (this != &rhs) {
		storage_ = rhs.storage_;
		type_    = rhs.type_;
	}

	return *this;
}

/**
 * @brief value::at
 * @param n
 * @return
 */
inline value value::at(std::size_t n) const {
	return as_array().at(n);
}

/**
 * @brief value::at
 * @param n
 * @return
 */
inline value &value::at(std::size_t n) {
	return as_array().at(n);
}

/**
 * @brief value::at
 * @param key
 * @return
 */
inline value value::at(std::string_view key) const {
	return as_object().at(key);
}

/**
 * @brief value::at
 * @param key
 * @return
 */
inline value &value::at(std::string_view key) {
	return as_object().at(key);
}

/**
 * @brief value::operator []
 * @param key
 * @return
 */
inline value value::operator[](std::string_view key) const {
	return as_object()[key];
}

/**
 * @brief value::operator []
 * @param n
 * @return
 */
inline value value::operator[](std::size_t n) const {
	return as_array()[n];
}

/**
 * @brief value::operator []
 * @param key
 * @return
 */
inline value &value::operator[](std::string_view key) {
	return as_object()[key];
}

/**
 * @brief value::operator []
 * @param n
 * @return
 */
inline value &value::operator[](std::size_t n) {
	return as_array()[n];
}

inline value value::operator[](const ptr &ptr) const {

	// this cast makes sure we don't get references to temps along the way
	// but the final return will create a copy
	value *result = const_cast<value *>(this);
	for (const std::string &ref : ptr) {

		if (result->is_object()) {
			result = &result->at(ref);
		} else if (result->is_array()) {

			if (ref == "-") {
				result->push_back(value());
				result = &result->at(result->size() - 1);
			} else {
				std::size_t n = std::stoul(ref);
				result        = &result->at(n);
			}
		} else {
			throw invalid_path();
		}
	}

	return *result;
}

inline value &value::operator[](const ptr &ptr) {

	value *result = this;
	for (const std::string &ref : ptr) {

		if (result->is_object()) {
			result = &result->at(ref);
		} else if (result->is_array()) {
			if (ref == "-") {
				result->push_back(value());
				result = &result->at(result->size() - 1);
			} else {
				std::size_t n = std::stoul(ref);
				result        = &result->at(n);
			}
		} else {
			throw invalid_path();
		}
	}

	return *result;
}

inline value &value::create(const ptr &ptr) {
	value *result = this;
	for (const std::string &ref : ptr) {

		if (result->is_object()) {
			if (!has_key(result, ref)) {
				result->insert(ref, object());
			}
			result = &result->at(ref);
		} else if (result->is_array()) {
			if (ref == "-") {
				result->push_back(value());
				result = &result->at(result->size() - 1);
			} else {
				std::size_t n = std::stoul(ref);
				result        = &result->at(n);
			}
		} else {
			throw invalid_path();
		}
	}

	return *result;
}

/**
 * @brief value::value
 * @param a
 */
inline value::value(const array &a)
	: type_(type_array) {
	storage_ = std::make_shared<array>(a);
}

/**
 * @brief value::value
 * @param o
 */
inline value::value(const object &o)
	: type_(type_object) {
	storage_ = std::make_shared<object>(o);
}

/**
 * @brief value::value
 * @param a
 */
inline value::value(array &&a)
	: type_(type_array) {
	storage_ = std::make_shared<array>(std::move(a));
}

/**
 * @brief value::value
 * @param o
 */
inline value::value(object &&o)
	: type_(type_object) {
	storage_ = std::make_shared<object>(std::move(o));
}

/**
 * @brief operator ==
 * @param lhs
 * @param rhs
 * @return
 */
inline bool operator==(const value &lhs, const value &rhs) {
	if (lhs.type_ == rhs.type_) {
		switch (lhs.type_) {
		case value::type_string:
			return as_string(lhs) == as_string(rhs);
		case value::type_number:
			// NOTE(eteran): this is kinda a "best effort", we can't compare the string
			// contents because things like 1e5 equals 100000
			return to_number<double>(lhs) == to_number<double>(rhs);
		case value::type_null:
			return true;
		case value::type_boolean:
			return to_bool(lhs) == to_bool(rhs);
		case value::type_array:
			return as_array(lhs) == as_array(rhs);
		case value::type_object:
			return as_object(lhs) == as_object(rhs);
		}
	}
	return false;
}

/**
 * @brief operator !=
 * @param lhs
 * @param rhs
 * @return
 */
inline bool operator!=(const value &lhs, const value &rhs) {
	return !(lhs == rhs);
}

/**
 * @brief operator ==
 * @param lhs
 * @param rhs
 * @return
 */
inline bool operator==(const object &lhs, const object &rhs) noexcept {
	if (lhs.values_.size() == rhs.values_.size()) {
		return lhs.values_ == rhs.values_;
	}
	return false;
}

/**
 * @brief operator !=
 * @param lhs
 * @param rhs
 * @return
 */
inline bool operator!=(const object &lhs, const object &rhs) noexcept {
	return !(lhs == rhs);
}

/**
 * @brief operator ==
 * @param lhs
 * @param rhs
 * @return
 */
inline bool operator==(const array &lhs, const array &rhs) noexcept {
	if (lhs.values_.size() == rhs.values_.size()) {
		return lhs.values_ == rhs.values_;
	}
	return false;
}

/**
 * @brief operator !=
 * @param lhs
 * @param rhs
 * @return
 */
inline bool operator!=(const array &lhs, const array &rhs) noexcept {
	return !(lhs == rhs);
}

template <class T>
void value::push_back(T &&v) {
	as_array().push_back(std::forward<T>(v));
}

template <class T>
void value::push_back(const T &v) {
	as_array().push_back(v);
}

template <class T>
std::pair<object::iterator, bool> value::insert(std::string key, const T &v) {
	return as_object().insert(std::move(key), v);
}

template <class T>
std::pair<object::iterator, bool> value::insert(std::string key, T &&v) {
	return as_object().insert(std::move(key), std::forward<T>(v));
}

template <class T>
std::pair<object::iterator, bool> value::insert(std::pair<std::string, T> &&p) {
	return as_object().insert(std::forward<T>(p));
}
}

#endif
