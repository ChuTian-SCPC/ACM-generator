#ifndef _SGPCET_SIMPLE_POLYGON_H_
#define _SGPCET_SIMPLE_POLYGON_H_

#ifndef _SGPCET_POINTS_H_
#include "points.h"
#endif //!_SGPCET_POINTS_H_
#ifndef _SGPCET_GEOMETRY_ALGORITHM_H_
#include "geometry_algorithm.h"
#endif //!_SGPCET_GEOMETRY_ALGORITHM_H_

namespace generator {
    namespace rand_geometry {

        template <typename T, typename = typename std::enable_if<is_point_type<T>::value>::type>
        class SimplePolygon;

        template <typename T, typename = typename std::enable_if<is_point_type<T>::value>::type>
        class SimplePolygonGen : public BasicPolygonGen<SimplePolygon, T> {
        public:
            using Context = SimplePolygon<T>;
            using Super = BasicPolygonGen<SimplePolygon, T>;
            SimplePolygonGen(Context& points) : BasicPolygonGen<SimplePolygon, T>(points) {}
        protected:
            virtual void __generate_geometry() override {
                _CONTEXT_GET(node_count);
                _CONTEXT_GET(x_left_limit);
                _CONTEXT_GET(x_right_limit);
                _CONTEXT_GET(y_left_limit);
                _CONTEXT_GET(y_right_limit);
                RandomPoints<T> p(node_count, x_left_limit, x_right_limit, y_left_limit, y_right_limit);
                p.gen();
                _CONTEXT_GET_REF(points);
                points = p.points();
                Point<T> center = __get_center(points);
                __polar_angle_sort(points, center);
            }

            Point<T> __get_center(std::vector<Point<T>>& points) {
                _ResultTypeT<T> x_sum = 0;
                _ResultTypeT<T> y_sum = 0;
                for (auto& p : points) {
                    x_sum += p.x();
                    y_sum += p.y();
                }
                return Point<T>(x_sum / points.size(), y_sum / points.size());
            }
        };

        template <typename T, typename>
        class SimplePolygon : public RandomPoints<T> {
        public:
            using _Self = SimplePolygon<T>;
            _OUTPUT_FUNCTION(_Self)
        public:
            SimplePolygon(int node_count = 1, T x_left_limit = 0, T x_right_limit = 0, T y_left_limit = 0, T y_right_limit = 0) :
                RandomPoints<T>(node_count, x_left_limit, x_right_limit, y_left_limit, y_right_limit) 
            {
                _GEOMETRY_DEFAULT 
            }

            _DISABLE_SAME_POINT
            _OUTPUT_FUNCTION_SETTING(_Self)
        protected:
            _DEFAULT_GEOMETRY_GEN_FUNC(SimplePolygon)
        };
    } // namespace rand_geometry
} // namespace generator

#endif //!_SGPCET_SIMPLE_POLYGON_H_
