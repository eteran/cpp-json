
#ifndef JSON_DETAIL_H_
#define JSON_DETAIL_H_

#include "json_error.h"
#include <cstdint>

namespace json {
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
		0x0, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

	if (static_cast<unsigned int>(ch) < 256) {
		return hex_val[static_cast<unsigned int>(ch)];
	} else {
		return 0;
	}
}

template <class Ch>
bool is_space(Ch ch) {
	return ch == '\t' || ch == '\n' || ch == '\r' || ch == ' ';
}

/**
 * @brief surrogate_pair_to_utf8
 * @param w1
 * @param w2
 * @param out
 */
template <class Out>
void surrogate_pair_to_utf8(std::uint16_t w1, std::uint16_t w2, Out &out) {

	std::uint32_t cp;
	if ((w1 & 0xfc00) == 0xd800) {
		if ((w2 & 0xfc00) == 0xdc00) {
			cp = 0x10000 + (((static_cast<std::uint32_t>(w1) & 0x3ff) << 10) | (w2 & 0x3ff));
		} else {
			JSON_THROW(invalid_unicode_character(0));
		}
	} else {
		cp = w1;
	}

	if (cp < 0x80) {
		*out++ = static_cast<std::uint8_t>(cp);
	} else if (cp < 0x0800) {
		*out++ = static_cast<std::uint8_t>(0xc0 | ((cp >> 6) & 0x1f));
		*out++ = static_cast<std::uint8_t>(0x80 | (cp & 0x3f));
	} else if (cp < 0x10000) {
		*out++ = static_cast<std::uint8_t>(0xe0 | ((cp >> 12) & 0x0f));
		*out++ = static_cast<std::uint8_t>(0x80 | ((cp >> 6) & 0x3f));
		*out++ = static_cast<std::uint8_t>(0x80 | (cp & 0x3f));
	} else if (cp < 0x1fffff) {
		*out++ = static_cast<std::uint8_t>(0xf0 | ((cp >> 18) & 0x07));
		*out++ = static_cast<std::uint8_t>(0x80 | ((cp >> 12) & 0x3f));
		*out++ = static_cast<std::uint8_t>(0x80 | ((cp >> 6) & 0x3f));
		*out++ = static_cast<std::uint8_t>(0x80 | (cp & 0x3f));
	}
}

}
}

#endif