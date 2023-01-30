
#ifndef JSON_20110525_H_
#define JSON_20110525_H_

#if defined(__EXCEPTIONS)
#define JSON_THROW(x) throw x
#else
#include <cassert>
#define JSON_THROW(...) assert(0)
#endif

#include "json_decode.h"
#include "json_detail.h"
#include "json_encode.h"
#include "json_error.h"
#include "json_ptr.h"
#include "json_value.h"

#endif
