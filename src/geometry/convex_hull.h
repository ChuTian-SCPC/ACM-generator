#ifndef _SGPCET_CONVEX_HULL_H_
#define _SGPCET_CONVEX_HULL_H_

#ifndef _SGPCET_POINTS_H_
#include "points.h"
#endif //!_SGPCET_POINTS_H_
#ifndef _SGPCET_GEOMETRY_ALGORITHM_H_
#include "geometry_algorithm.h"
#endif //!_SGPCET_GEOMETRY_ALGORITHM_H_

namespace generator {
    namespace rand_geometry {
    
        template <typename T, typename = typename std::enable_if<is_point_type<T>::value>::type>
        class ConvexHull;

        template <typename T, typename = typename std::enable_if<is_point_type<T>::value>::type>
        class ConvexHullGen : public BasicPolygonGen<ConvexHull, T> {
        public:
            using Context = ConvexHull<T>;
            using Super = BasicPolygonGen<ConvexHull, T>;
            ConvexHullGen(Context& points) : BasicPolygonGen<ConvexHull, T>(points) {}
        protected:
            virtual void __judge_self_limit() override {
                Super::__judge_self_limit();
            }

            void __judge_max_try() {
                _CONTEXT_GET(max_try);
                if (max_try <= 0) {
                    _msg::__fail_msg(_msg::_defl,
                        tools::string_format("max_try should be greater than 0, but found %d.", max_try));
                }
            }

            void __rand_pool_to_vector(std::vector<T>& pool, std::vector<T>& vec) {
                _CONTEXT_GET(node_count);
                std::sort(pool.begin(), pool.end());
                T min = pool.front();
                T max = pool.back();
                T lower = min;
                T upper = min;
                for (int i = 1; i < node_count - 1; i++) {
                    T val = pool[i];
                    if (rand_numeric::rand_bool()) {
                        vec.emplace_back(val - lower);
                        lower = val;
                    }
                    else {
                        vec.emplace_back(upper - val);
                        upper = val;
                    }
                }
                vec.emplace_back(max - lower);
                vec.emplace_back(upper - max);
            }

            virtual void __generate_geometry() override {
                _CONTEXT_GET(max_try)
                int try_time = 0;
                bool success = false;
                while(try_time < max_try) {
                    try_time++;
                    success = __try_generate_once();
                    if (success) break;
                }
                if (!success) {
                    _msg::__fail_msg(_msg::_defl,
                        tools::string_format("Tried %d times, found no convex hull satisfied the condition.", max_try));
                }
            }

            bool __try_generate_once() {
                _CONTEXT_GET(node_count)
                std::vector<T> x_pool;
                std::vector<T> y_pool;
                for (int i = 0; i < node_count; i++) {
                    x_pool.emplace_back(this->__rand_x());
                    y_pool.emplace_back(this->__rand_y());
                }
                std::vector<T> x_vec;
                std::vector<T> y_vec;
                __rand_pool_to_vector(x_pool, x_vec);
                __rand_pool_to_vector(y_pool, y_vec);
                if (__zero_count_overflow(x_vec, y_vec)) return false;
                std::vector<Point<T>> vec = __rand_no_origin_points(x_vec, y_vec);
                __polar_angle_sort(vec);
                T min_x = std::numeric_limits<T>::max();
                T max_x = std::numeric_limits<T>::min();
                T min_y = std::numeric_limits<T>::max();
                T max_y = std::numeric_limits<T>::min();
                std::vector<Point<T>> hull;
                Point<T> o;
                for (auto& v : vec) {
                    o += v;
                    hull.emplace_back(o);
                    min_x = std::min(min_x, o.x());
                    max_x = std::max(max_x, o.x());
                    min_y = std::min(min_y, o.y());
                    max_y = std::max(max_y, o.y());
                }
                Point<T> move = __rand_move_point(min_x, max_x, min_y, max_y);
                _CONTEXT_GET_REF(points)
                for (auto &v : hull) {
                    points.emplace_back(v + move);
                }
                return true;
            }

            bool __zero_count_overflow(std::vector<T>& x_vec, std::vector<T>& y_vec) {
                _CONTEXT_GET(node_count)
                int count = 0;
                for (auto p : x_vec) count += (p == 0);
                for (auto p : y_vec) count += (p == 0);
                return count > node_count;
            }

            int __find_none_zero_index(std::vector<T>& vec) {
                for (int i = 0; i < vec.size(); i++) {
                    if (vec[i] != 0) return i;
                }
                return vec.size();
            }

            std::vector<Point<T>> __rand_no_origin_points(std::vector<T>& x_vec, std::vector<T>& y_vec) {
                std::vector<Point<T>> points;
                std::sort(x_vec.begin(), x_vec.end());
                std::sort(y_vec.begin(), y_vec.end());
                int p_x = __find_none_zero_index(x_vec);
                int p_y = __find_none_zero_index(y_vec);
                shuffle(x_vec.begin() + p_x, x_vec.end());
                shuffle(y_vec.begin() + p_y, y_vec.end());
                std::reverse(y_vec.begin(), y_vec.end());
                for (int i = 0; i < x_vec.size(); i++) {
                    points.emplace_back(x_vec[i], y_vec[i]);
                }
                return points;
            }
            
            Point<T> __rand_move_point(T min_x, T max_x, T min_y, T max_y) {
               _CONTEXT_GET(x_left_limit)
               _CONTEXT_GET(x_right_limit)
               _CONTEXT_GET(y_left_limit)
               _CONTEXT_GET(y_right_limit)
               T x_min_move = x_left_limit - min_x;
               T x_max_move = x_right_limit - max_x;
               T y_min_move = y_left_limit - min_y;
               T y_max_move = y_right_limit - max_y;
               return rand_point(x_min_move, x_max_move, y_min_move, y_max_move);
            }

        };

        template <typename T, typename>
        class ConvexHull : public RandomPoints<T> {
        protected:
            int _max_try; 
        public:
            using _Self = ConvexHull<T>;
            _OUTPUT_FUNCTION(_Self)
        public:
            ConvexHull(int node_count = 1, T x_left_limit = 0, T x_right_limit = 0, T y_left_limit = 0, T y_right_limit = 0) :
                RandomPoints<T>(node_count, x_left_limit, x_right_limit, y_left_limit, y_right_limit), _max_try(10)
            {
                _GEOMETRY_DEFAULT
            }

            _DISABLE_SAME_POINT
            _SET_GET_VALUE(int, max_try)

            _OUTPUT_FUNCTION_SETTING(_Self)
        protected:
            _DEFAULT_GEOMETRY_GEN_FUNC(ConvexHull)
        };
    } // namespace rand_geometry
} // namespace generator

#endif //!_SGPCET_CONVEX_HULL_H_
