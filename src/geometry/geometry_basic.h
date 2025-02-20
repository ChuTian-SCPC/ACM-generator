#ifndef _SGPCET_GEOMETRY_BASIC_H_
#define _SGPCET_GEOMETRY_BASIC_H_

#ifndef _SGPCET_COMMON_H_
#include "basic/common.h"
#endif //!_SGPCET_COMMON_H_
#ifndef _SGPCET_LOGGER_H_
#include "log/logger.h"
#endif // !_SGPCET_NODE_H_

namespace generator {
    namespace rand_geometry {
        #if defined(__SIZEOF_INT128__) && __SIZEOF_INT128__ == 16
            using MaxIntType = __int128;
            std::ostream& operator<<(std::ostream& os, __int128 value) {
                if (value < 0) {
                    os << "-";
                    value = -value;
                }

                std::string str;
                while (value > 0) {
                    str.insert(str.begin(), '0' + (value % 10));
                    value /= 10;
                }

                if (str.empty()) {
                    str = "0";
                }

                os << str;
                return os;
            }
        #else
            using MaxIntType = long long;
        #endif
        
        template<typename T>
        struct is_signed_integral {
            static const bool value = std::is_integral<T>::value && !std::is_unsigned<T>::value;
        };
        
        template<typename T>
        struct is_point_type {
            static const bool value = is_signed_integral<T>::value || std::is_floating_point<T>::value;
        };
        
        template<typename T>
        struct _ResultType {
            using type = typename std::conditional<
                is_signed_integral<T>::value,   
                MaxIntType,
                double 
            >::type;
        };
        
        template<typename T>
        using _ResultTypeT = typename _ResultType<T>::type; 
    } // namespace rand_geometry
} // namespace generator

#endif //!_SGPCET_GEOMETRY_BASIC_H_
