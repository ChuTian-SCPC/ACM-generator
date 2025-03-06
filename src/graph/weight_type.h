#ifndef _SGPCET_WEIGHT_TYPE_H_
#define _SGPCET_WEIGHT_TYPE_H_

#ifndef _SGPCET_COMMON_H_
#include "basic/common.h"
#endif // !_SGPCET_COMMON_H_

namespace generator {
    namespace rand_graph {
        namespace basic {
            template<typename T, typename U>
            using _IsBothWeight = typename std::enable_if<
                !std::is_void<T>::value && !std::is_void<U>::value, int>::type;

            template<typename T, typename U>
            using _IsEdgeWeight = typename std::enable_if<
                std::is_void<T>::value && !std::is_void<U>::value, int>::type;

            template<typename T, typename U>
            using _IsNodeWeight = typename std::enable_if<
                !std::is_void<T>::value && std::is_void<U>::value, int>::type;

            template<typename T, typename U>
            using _IsUnweight = typename std::enable_if<
                std::is_void<T>::value && std::is_void<U>::value, int>::type;

            template<typename T>
            using _HasT = typename std::enable_if<!std::is_void<T>::value, int>::type;

            template<typename T>
            using _NotHasT = typename std::enable_if<std::is_void<T>::value, int>::type;

        } // namespace basic
    } // namespace rand_graph
} // namespace generator

#endif // !_SGPCET_WEIGHT_TYPE_H_
