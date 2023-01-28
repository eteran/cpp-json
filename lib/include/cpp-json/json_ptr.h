
#ifndef JSON_PTR_H_
#define JSON_PTR_H_

#include "Reader.h"
#include "json_detail.h"
#include "json_error.h"
#include <string>
#include <string_view>
#include <vector>

namespace json {

class value;

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

}

#endif
