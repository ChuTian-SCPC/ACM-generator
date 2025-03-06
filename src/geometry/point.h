#ifndef _SGPCET_POINT_H_
#define _SGPCET_POINT_H_

#ifndef _SGPCET_COORDINATE_H_
#include "coordinate.h"
#endif //!_SGPCET_COORDINATE_H_

namespace generator {
    namespace rand_geometry {

        template <typename T, typename = typename std::enable_if<is_point_type<T>::value>::type> 
        class _2Points;

        template <typename T, typename = typename std::enable_if<is_point_type<T>::value>::type>
        class Point : public Coordinate<T> {
        protected:
            using _Self = Point<T>;
            _OUTPUT_FUNCTION(_Self)
        
        public:
            Point() : Coordinate<T>(0, 0) {
                _DEFAULT_OUTPUT
            }

            Point(T x, T y) : Coordinate<T>(x, y) { 
                _DEFAULT_OUTPUT
            }

            Point(const Point<T>& p) : Coordinate<T>(p) {
                _DEFAULT_OUTPUT
            }
            Point(Point<T>&& p) noexcept : Coordinate<T>(std::move(p)) {
                _DEFAULT_OUTPUT
            }
            Point& operator=(const Point<T>& p) {
                if (this != &p) 
                    Coordinate<T>::operator=(p);
                return *this;
            }
            Point& operator=(Point<T>&& p) noexcept {
                if (this!= &p)
                    Coordinate<T>::operator=(std::move(p));
                return *this;
            }
            Point(const _2Points<T>& p) : Coordinate<T>(p.end() - p.start()) {
                _DEFAULT_OUTPUT
            }
            
            Point operator+(const Point& b) const { return Point(this->_x + b._x, this->_y + b._y); }
            Point& operator+=(const Point& b) {
                this->_x += b._x;
                this->_y += b._y;
                return *this;
            }
            Point operator-(const Point& b) const { return Point(this->_x - b._x, this->_y - b._y); }
            Point& operator-=(const Point& b) {
                this->_x -= b._x;
                this->_y -= b._y;
                return *this;
            }
            T& operator[](int idx) {
                if (idx == 0) return this->_x;
                if (idx == 1) return this->_y;
                _msg::__fail_msg(_msg::_defl, "index is out of range.");
            }
            T& operator[](char c) {
                if (c == 'x' || c == 'X') return this->_x;
                if (c == 'y' || c == 'Y') return this->_y;
                _msg::__fail_msg(_msg::_defl, "index is out of range.");

            }
            T& operator[](std::string s) {
                if(s.empty()) _msg::__fail_msg(_msg::_defl,"index s is an empty string.");
                return this->operator[](s[0]);
            }
            bool operator==(const Point<T>& p) const{ return this->_x == p._x && this->_y == p._y; }
            bool operator!=(const Point<T>& p) const{ return !(*this == p); }
            bool operator<(const Point<T>& p) const{ return this->_x < p._x || (this->_x == p._x && this->_y < p._y); }
            bool operator<=(const Point<T>& p) const{ return *this < p || *this == p; }
            bool operator>(const Point<T>& p) const { return !(*this <= p); }
            bool operator>=(const Point<T>& p) const { return !(*this < p); }

            _ResultTypeT<T> operator^(const Point& b) const{ 
                _ResultTypeT<T> x1 = this->x();
                _ResultTypeT<T> y1 = this->y();
                _ResultTypeT<T> x2 = b.x();
                _ResultTypeT<T> y2 = b.y();
                return x1 * y2 - y1 * x2;
            }
            
            _ResultTypeT<T> operator*(const Point& b) const{ 
                _ResultTypeT<T> x1 = this->x();
                _ResultTypeT<T> y1 = this->y();
                _ResultTypeT<T> x2 = b.x();
                _ResultTypeT<T> y2 = b.y();
                return x1 * x2 + y1 * y2;
            }

            _GEOMETRY_IN_RAND_FUNC(RandomCoordinate)
            _OUTPUT_FUNCTION_SETTING(_Self)
        protected:
            void __rand(RandomCoordinate<T>& c) {
                c.gen();
                this->_x = c.x();
                this->_y = c.y();
            }
        };

        _GEOMETRY_OUT_RAND_FUNC(rand_point, Point)        
    }    // namespace rand_geometry
}    // namespace generator
#endif // !_SGPCET_POINT_H_
