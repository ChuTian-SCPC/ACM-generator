#ifndef _SGPCET_COORDINATE_H_
#define _SGPCET_COORDINATE_H_

#ifndef _SGPCET_GEOMETRY_STRATEGY_H_
#include "geometry_strategy.h"
#endif //!_SGPCET_GEOMETRY_STRATEGY_H_
#ifndef _SGPCET_NUMERIC_H_
#include "rand/numeric.h"
#endif //!_SGPCET_NUMERIC_H_
#ifndef _SGPCET_RANGE_FORMAT_H_
#include "range_format.h"
#endif //!_SGPCET_RANGE_FORMAT_H_

namespace generator {
    namespace rand_geometry {
    
        template<typename T, typename = typename std::enable_if<is_point_type<T>::value>::type>
        class Coordinate {
        protected:
            T _x, _y;

            using _Self = Coordinate<T>;
            _OUTPUT_FUNCTION(_Self)
        public:
            Coordinate(T x = 0, T y = 0) : _x(x), _y(y) {
                _DEFAULT_OUTPUT
            }

            Coordinate(const Coordinate<T>& p) : _x(p._x), _y(p._y) {
                _DEFAULT_OUTPUT
            }

            Coordinate(Coordinate<T>&& p) noexcept : _x(std::move(p._x)), _y(std::move(p._y)) {
                _DEFAULT_OUTPUT
            }

            Coordinate<T>& operator=(const Coordinate<T>& p) {
                if (this != &p) {
                    _x = p._x;
                    _y = p._y;
                }
                return *this;
            }

            Coordinate<T>& operator=(Coordinate<T>&& p) noexcept {
                if (this != &p) {
                    _x = std::move(p._x);
                    _y = std::move(p._y);
                }
                return *this;
            } 

            _SET_GET_VALUE(T, x);
            _SET_GET_VALUE(T, y);

            void default_output(std::ostream& os) const {
                os << _x << " " << _y;
            }

            _OUTPUT_FUNCTION_SETTING(_Self)
        };

        template<typename T, typename = typename std::enable_if<is_point_type<T>::value>::type>
        class LimitRange {
        protected:
            T _x_left_limit, _x_right_limit;
            T _y_left_limit, _y_right_limit;
        public:
            LimitRange(T x_left_limit = 0, T x_right_limit = 0, T y_left_limit = 0, T y_right_limit = 0) :
                _x_left_limit(x_left_limit), _x_right_limit(x_right_limit),
                _y_left_limit(y_left_limit), _y_right_limit(y_right_limit) {}

            _SET_GET_VALUE(T, x_left_limit);
            _SET_GET_VALUE(T, x_right_limit);
            _SET_GET_VALUE(T, y_left_limit);
            _SET_GET_VALUE(T, y_right_limit);

            void set_x_limit(T left_limit, T right_limit) {
                _x_left_limit = left_limit;
                _x_right_limit = right_limit;
            }

            void set_x_limit(std::string format) {
                auto range = rand_numeric::__format_to_range<T>(format);
                _x_left_limit = range.first;
                _x_right_limit = range.second;
            }

            void set_y_limit(T left_limit, T right_limit) {
                _y_left_limit = left_limit;
                _y_right_limit = right_limit;  
            }

            void set_y_limit(std::string format) {
                auto range = rand_numeric::__format_to_range<T>(format);
                _y_left_limit = range.first;
                _y_right_limit = range.second; 
            }

            void set_xy_limit(T left, T right) {
                set_x_limit(left, right);
                set_y_limit(left, right);
            }

            void set_xy_limit(T x_left_limit, T x_right_limit, T y_left_limit, T y_right_limit) {
                set_x_limit(x_left_limit, x_right_limit);
                set_y_limit(y_left_limit, y_right_limit);
            }

            void set_xy_limit(std::string format) {
                auto range = __format_xy_range(format);
                set_x_limit(range.first);
                set_y_limit(range.second);
            }
        };

        template<typename T, typename = typename std::enable_if<is_point_type<T>::value>::type>
        class RandomCoordinate;

        template<typename T>
        class RandomCoordinateGen : public BasicGeometryGen<RandomCoordinate, T> {
        public:
            using Context = RandomCoordinate<T>;
            RandomCoordinateGen(Context& coordinate) : BasicGeometryGen<RandomCoordinate, T>(coordinate) {} 
        protected:
            virtual void __generate_geometry() override {
                _CONTEXT_GET_REF(x);
                _CONTEXT_GET_REF(y);
                _CONTEXT_GET(x_left_limit);
                _CONTEXT_GET(x_right_limit);
                _CONTEXT_GET(y_left_limit);
                _CONTEXT_GET(y_right_limit);
                x = rand_numeric::rand_int<T>(x_left_limit, x_right_limit);
                y = rand_numeric::rand_int<T>(y_left_limit, y_right_limit);
            }

        };

        template<typename T, typename>
        class RandomCoordinate : public Coordinate<T>, public LimitRange<T>, public _GeometryGenSwitch {
        protected:
            using _Self = RandomCoordinate<T>;
            _OUTPUT_FUNCTION(_Self)
        public:
            RandomCoordinate(T x_left_limit = 0, T x_right_limit = 0, T y_left_limit = 0, T y_right_limit = 0) :
                Coordinate<T>(), 
                LimitRange<T>(x_left_limit, x_right_limit, y_left_limit, y_right_limit),
                _GeometryGenSwitch() 
            {
                _GEOMETRY_DEFAULT
            }

            void gen() { 
                this->_generator->generate(); 
            }

            _OUTPUT_FUNCTION_SETTING(_Self)
        protected:
            _DEFAULT_GEOMETRY_GEN_FUNC(RandomCoordinate)
        };

    } // namespace rand_geometry
} // namespace generator

#endif // !_SGPCET_COORDINATE_H_
