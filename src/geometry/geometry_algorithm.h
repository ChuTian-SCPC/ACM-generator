#ifndef _SGPCET_GEOMETRY_ALGORITHM_H_
#define _SGPCET_GEOMETRY_ALGORITHM_H_

#ifndef _SGPCET_POINT_H_
#include "point.h"
#endif //!_SGPCET_POINT_H_
#ifndef _SGPCET_LINE_SEGMENT_H_
#include "line_segment.h"
#endif //!_SGPCET_LINE_SEGMENT_H_

namespace generator {
    namespace rand_geometry {
    
    template <typename T>
    typename std::enable_if<is_point_type<T>::value, int>::type
    __quadrant(Point<T> p) {
        return ((p.y() < 0) << 1) | ((p.x() < 0) ^ (p.y() < 0));
    }

    template <typename T>
    typename std::enable_if<is_point_type<T>::value, void>::type        
    __polar_angle_sort(std::vector<Point<T>>& points, Point<T> o = Point<T>()) {
        std::sort(points.begin(), points.end(), [&](Point<T> a, Point<T> b) {
            Point<T> oa = a - o;
            Point<T> ob = b - o;
            int quadrant_a = __quadrant(oa);
            int quadrant_b = __quadrant(ob);
            if (quadrant_a == quadrant_b) {
                _ResultTypeT<T> cross = oa ^ ob;
                if (cross == 0) return a.x() < b.x();
                return cross > 0;
            }
            return quadrant_a < quadrant_b;
        });
    }

    template <typename T>
    _enum::PointDirection point_direction(Point<T> a, Point<T> b, Point<T> c) {
        b = b - a;
        c = c - a;
        _ResultTypeT<T> cross = b ^ c;
        if (cross > 0) return _enum::COUNTER_CLOCKWISE;
        if (cross < 0) return _enum::CLOCKWISE;
        if (b * c < 0) return _enum::ONLINE_BACK;
        if (b * b < c * c) return _enum::ONLINE_FRONT;
        return _enum::ON_SEGMENT;
    }
    
    template <typename T>
    _enum::PointDirection point_direction(Point<T> a, Segment<T> s) {
        return point_direction(a, s.start, s.end);
    }

    } // namespace rand_geometry
} // namespace generator

#endif // !_SGPCET_GEOMETRY_ALGORITHM_H_
