#ifndef _SGPCET_COORDINATE_H_
#define _SGPCET_COORDINATE_H_

#ifndef _SGPCET_GEOMETRY_STRATEGY_H_
#include "geometry_strategy.h"
#endif //!_SGPCET_GEOMETRY_STRATEGY_H_
#ifndef _SGPCET_NUMERIC_H_
#include "rand/numeric.h"
#endif //!_SGPCET_NUMERIC_H_

namespace generator {
    namespace rand_geometry {
    
        template<typename T>
        class Coordinate {
        protected:
            T _x, _y;
        public:
            Coordinate(T x = 0, T y = 0) : _x(x), _y(y) {}

            _SET_GET_VALUE(T, x);
            _SET_GET_VALUE(T, y);
        };
    } // namespace rand_geometry
} // namespace generator

#endif // !_SGPCET_COORDINATE_H_
