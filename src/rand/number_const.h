#ifndef _SGPCET_NUMBER_CONST_H_
#define _SGPCET_NUMBER_CONST_H_

#ifndef _SGPCET_COMMON_H_
#include "basic/common.h"
#endif // !_SGPCET_COMMON_H_
#ifndef _SGPCET_ENUM_H_
#include "basic/enum.h"
#endif // !_SGPCET_ENUM_H_

namespace generator {
    namespace rand_numeric {
        const long long __LONG_LONG_MIN = std::numeric_limits<long long>::min();
        const unsigned long long __UNSIGNED_LONG_LONG_MAX = std::numeric_limits<unsigned long long>::max();
        const unsigned long long __UNSIGNED_LONG_LONG_MIN = std::numeric_limits<unsigned long long>::min();
        const unsigned long long __CHECK_LONG_LONG_MAX = (unsigned long long)std::numeric_limits<long long>::max();
        const unsigned long long __CHECK_ABS_LONG_LONG_MIN = __CHECK_LONG_LONG_MAX + 1ULL;
        const unsigned long long __CHECK_UNSIGNED_LONG_MAX = (unsigned long long)std::numeric_limits<unsigned long>::max();
    
        template <typename T>
        struct IsNumeric {
            static constexpr bool value = std::is_integral<T>::value || std::is_floating_point<T>::value;
        };

        const std::string _PATTERN[_enum::MaxCharType]={
            "[a-z]",
            "[A-Z]",
            "[a-zA-Z]",
            "[0-9]",
            "[a-zA-Z0-9]",
            "[01]"
        };
    } // namespace rand_numeric
} // namespace generator

#endif // !_SGPCET_NUMBER_CONST_H_
