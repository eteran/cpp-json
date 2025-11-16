#ifndef CPP_JSON_CONFIG_H_
#define CPP_JSON_CONFIG_H_

#include "cxx_universal.h"

#ifdef CPP_JSON_CONDITIONAL_EXPLICIT_SUPPORTED
#define CPP_JSON_EXPLICIT_IF(...) explicit(__VA_ARGS__)
#else
#define CPP_JSON_EXPLICIT_IF(...)
#endif

#ifdef CPP_JSON_STATIC_CALL_OPERATOR_SUPPORTED
#define CPP_JSON_STATIC_CALLOP static
#define CPP_JSON_OR_CONST_CALLOP
#else
#define CPP_JSON_STATIC_CALLOP
#define CPP_JSON_OR_CONST_CALLOP const
#endif

#endif
