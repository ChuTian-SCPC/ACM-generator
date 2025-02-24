#ifndef _SGPCET_POINTS_H_
#define _SGPCET_POINTS_H_

#ifndef _SGPCET_POINT_H_
#include "point.h"
#endif //!_SGPCET_POINT_H_

namespace generator {
    namespace rand_geometry {
        
        template <typename T, typename = typename std::enable_if<is_point_type<T>::value>::type>
        class RandomPoints;

        template <typename T, typename = typename std::enable_if<is_point_type<T>::value>::type>
        class RandomPointsGen : public BasicGeometryGen<RandomPoints, T> {
        protected:
            std::set<Point<T>> _p;
        public:
            using Context = RandomPoints<T>;
            RandomPointsGen(Context& points) : BasicGeometryGen<RandomPoints, T>(points) {}
        public:
            virtual void __generate_geometry() override {
                _CONTEXT_GET(x_left_limit);
                _CONTEXT_GET(x_right_limit);
                _CONTEXT_GET(y_left_limit);
                _CONTEXT_GET(y_right_limit);
                _CONTEXT_GET(node_count);
                for (int i = 0; i < node_count; i++) {
                    Point<T> p;
                    do {
                        p = rand_point<T>(x_left_limit, x_right_limit, y_left_limit, y_right_limit);
                    } while (__judge_same_point(p));
                    __add_point(p);
                }
            }

        protected:
            bool __judge_same_point(Point<T>& p) {
                if (_CONTEXT_V(same_point)) return false;
                return _p.find(p) != _p.end();
            }

            void __add_point(Point<T>& p) {
                if (!_CONTEXT_V(same_point)) _p.insert(p);
                _CONTEXT_GET_REF(points);
                points.push_back(p);
            }
            virtual void __self_init() override {
                if (!_CONTEXT_V(same_point)) _p.clear();
            }

            virtual void __judge_self_limit() override{
                _CONTEXT_GET(node_count);
                if (node_count <= 0) {
                    _msg::__fail_msg(_msg::_defl, 
                        tools::string_format("node_count should be greater than 0, but found %d.", node_count));                    
                }
                if (node_count > _setting::node_limit) {
                    _msg::__fail_msg(_msg::_defl,
                        tools::string_format("node_count should be less than node_limit(%d), but found %d.", _setting::node_limit, node_count));
                }
                if (!_CONTEXT_V(same_point)) __judge_max_node_count();
            }

            template<typename F = T, typename std::enable_if<std::is_floating_point<F>::value, int>::type = 0>
            void __judge_max_node_count() {}

            template<typename F = T, typename std::enable_if<is_signed_integral<F>::value, int>::type = 0>
            void __judge_max_node_count() {
                _CONTEXT_GET(node_count);
                _CONTEXT_GET(x_left_limit);
                _CONTEXT_GET(x_right_limit);
                _CONTEXT_GET(y_left_limit);
                _CONTEXT_GET(y_right_limit);
                T x_range = x_right_limit - x_left_limit + 1;
                T y_range = y_right_limit - y_left_limit + 1;
                if (x_range >= node_count || y_range >= node_count) return;
                T max_node_count = x_range * y_range;
                if (max_node_count >= node_count) return;
                _msg::__fail_msg(_msg::_defl,
                    tools::string_format("node_count should be less than or equal to %d, but found %d.", max_node_count, node_count));
            }
        };

        template <typename T, typename>
        class RandomPoints : public RandomCoordinate<T> {
        protected:
            int _node_count;
            std::vector<Point<T>> _points;
            bool _same_point;
            bool _output_node_count;
        public:
            using _Self = RandomPoints<T>;
            _OUTPUT_FUNCTION(_Self)
        public:
            RandomPoints(int node_count = 1, T x_left_limit = 0, T x_right_limit = 0, T y_left_limit = 0, T y_right_limit = 0) : 
                RandomCoordinate<T>(x_left_limit, x_right_limit, y_left_limit, y_right_limit), 
                _node_count(node_count), _same_point(false), _output_node_count(true)
            {
                _GEOMETRY_DEFAULT
            }
            
            _SET_GET_VALUE(int, node_count);
            _SET_GET_VALUE(bool, output_node_count);
            _SET_GET_VALUE(bool, same_point);
            _SET_GET_VALUE(std::vector<Point<T>>, points);

            void default_output(std::ostream& os) const {
                if (_output_node_count) {
                    os << _node_count << "\n";
                }
                int points_count = 0;
                for (auto p : _points) {
                    os << p;
                    if (++points_count < _node_count) {
                        os << "\n";
                    }
                }
            }

            _OUTPUT_FUNCTION_SETTING(_Self)
        protected:
            _DEFAULT_GEOMETRY_GEN_FUNC(RandomPoints)
        };
    } // namespace rand_geometry
} // namespace generator

#endif //!_SGPCET_POINTS_H_
