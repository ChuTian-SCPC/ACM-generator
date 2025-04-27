#ifndef _SGPCET_TRIANGLE_H_
#define _SGPCET_TRIANGLE_H_

#ifndef _SGPCET_CONVEX_HULL_H_
#include "convex_hull.h"
#endif //!_SGPCET_CONVEX_HULL_H_

namespace generator {
    namespace rand_geometry {
        template <typename T, typename = typename std::enable_if<is_point_type<T>::value>::type>
        class Triangle;

        template <typename T, typename = typename std::enable_if<is_point_type<T>::value>::type>
        class TriangleGen : public ConvexHullGen<T>  {
        public:
            using Context = Triangle<T>;
            TriangleGen(Context& points) : ConvexHullGen<T>(points) {}
        };

        template <typename T, typename>
        class Triangle : public ConvexHull<T> {
        public:
            using _Self = Triangle<T>;
            _OUTPUT_FUNCTION(_Self);
        public:
            Triangle(T x_left_limit = 0, T x_right_limit = 0, T y_left_limit = 0, T y_right_limit = 0) :
                ConvexHull<T>(3, x_left_limit, x_right_limit, y_left_limit, y_right_limit)       
            {
                _GEOMETRY_DEFAULT 
                this->_output_node_count = false;
            }

            void default_output(std::ostream& os) const {
                if (this->_output_node_count) {
                    os << this->_node_count << "\n";
                }
                os << this->_points[0] << " " << this->_points[1] << " " << this->_points[2];
            }

            _DISABLE_NODE_COUNT
            _OUTPUT_FUNCTION_SETTING(_Self)
        protected:
            _DEFAULT_GEOMETRY_GEN_FUNC(Triangle)
        };
    } // namespace rand_geometry
} // namespace generator
#endif //!_SGPCET_TRIANGLE_H_
