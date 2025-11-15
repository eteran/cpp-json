#pragma once

#include "cxx_universal.h"

#ifdef CPP_JSON_STATIC_CALL_OPERATOR_SUPPORTED
#define CPP_JSON_STATIC_CALLOP static
#define CPP_JSON_OR_CONST_CALLOP
#else
#define CPP_JSON_STATIC_CALLOP
#define CPP_JSON_OR_CONST_CALLOP const
#endif
