#ifndef _SGPCET_LINE_SEGMENT_H_
#define _SGPCET_LINE_SEGMENT_H_

#ifndef _SGPCET_POINTS_H_
#include "points.h"
#endif //!_SGPCET_POINTS_H_

namespace generator {
    namespace rand_geometry {
        template <typename T, typename>
        class _2Points {
        protected:
            using _Self = _2Points<T>;
            _OUTPUT_FUNCTION(_Self)
            Point<T> _start, _end;
        public:
            _2Points() {
                _DEFAULT_OUTPUT
            }

            _2Points(const Point<T>& start, const Point<T>& end) : _start(start), _end(end) {
                _DEFAULT_OUTPUT
            }

            _SET_GET_VALUE(Point<T>, start)
            _SET_GET_VALUE(Point<T>, end)

            Point<T> to_vector() { return _end - _start; }

            _ResultTypeT<T> operator^(const Point<T>& b) { return (_end - _start) ^ b; }
            _ResultTypeT<T> operator^(const _2Points<T>& l) { return (_end - _start) ^ (l._end - l._start); }
            _ResultTypeT<T> operator*(const Point<T>& b) { return (_end - _start) * b; }
            _ResultTypeT<T> operator*(const _2Points<T>& l) { return (_end - _start) * (l._end - l._start); }        

            void default_output(std::ostream& os) const {
                os << _start << " " << _end;
            }

            _GEOMETRY_IN_RAND_FUNC(RandomPoints)
            _OUTPUT_FUNCTION_SETTING(_Self)
        protected:
            void __rand(RandomPoints<T>& p) {
                p.set_node_count(2);
                p.set_same_point(false);
                p.gen();
                auto points = p.points();
                _start = points[0];
                _end = points[1];
            }
        }; 

        template <typename T, typename = typename std::enable_if<is_point_type<T>::value>::type>
        class Line : public _2Points<T> {
        protected:
            using _Self = Line<T>;
            _OUTPUT_FUNCTION(_Self)
        public:
            Line() : _2Points<T>() {
                _DEFAULT_OUTPUT
            }
            Line(Point<T> start, Point<T> end) : _2Points<T>(start, end) {
                _DEFAULT_OUTPUT
            }
            
            _OUTPUT_FUNCTION_SETTING(_Self)
        };

        _GEOMETRY_OUT_RAND_FUNC(rand_line, Line)
        
        template <typename T, typename = typename std::enable_if<is_point_type<T>::value>::type>
        class Segment : public _2Points<T> {
        protected:
            using _Self = Segment<T>;
            _OUTPUT_FUNCTION(_Self)
        public:
            Segment() : _2Points<T>() {
                _DEFAULT_OUTPUT
            }
            Segment(Point<T> start, Point<T> end) : _2Points<T>(start, end) {
                _DEFAULT_OUTPUT
            }
            
            _OUTPUT_FUNCTION_SETTING(_Self)
        };

        _GEOMETRY_OUT_RAND_FUNC(rand_segment, Segment)
    } // namespace rand_geometry
} // namespace generator

#endif // !_SGPCET_LINE_SEGMENT_H_
