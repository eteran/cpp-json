#ifndef CPP_JSON_CXX_CORE_FEATURES_H_
#define CPP_JSON_CXX_CORE_FEATURES_H_

/*
 *  This header detects optional C++ core language features.
 */

#if __cpp_exceptions >= 199711L
#define CPP_JSON_EXCEPTIONS_SUPPORTED
#endif

#if __cpp_conditional_explicit >= 201806L
#define CPP_JSON_CONDITIONAL_EXPLICIT_SUPPORTED
#endif

#if __cpp_static_call_operator >= 202207L
#define CPP_JSON_STATIC_CALL_OPERATOR_SUPPORTED
#endif

#if __cplusplus >= 202002L
#define CPP_JSON_DEFAULT_COMPARISONS_SUPPORTED
#endif

#endif
