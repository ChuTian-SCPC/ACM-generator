#ifndef _SGPCET_EDGE_H_
#define _SGPCET_EDGE_H_

#ifndef _SGPCET_MACRO_H_
#include "basic/macro.h"
#endif // !_SGPCET_MACRO_H_

namespace generator {
    namespace rand_graph {
        namespace _basic {
            class _BasicEdge {
            protected:
                int _u, _v;
                bool _swap_node;
            public:
                _BasicEdge(int u, int v) : _u(u), _v(v), _swap_node(false) {}

                _SET_GET_VALUE(int, u)
                _SET_GET_VALUE(int, v)
                _SET_GET_VALUE(bool, swap_node)

                friend bool operator==(const _BasicEdge a, const _BasicEdge b) {
                    return a._u == b._u && a._v == b._v;
                }
                friend bool operator!=(const _BasicEdge a, const _BasicEdge b) {
                    return !(a == b);
                }
                friend bool operator<(const _BasicEdge a, const _BasicEdge b) {
                    if (a._u == b._u) return a._v < b._v;
                    return a._u < b._u;
                }
                friend bool operator<=(const _BasicEdge a, const _BasicEdge b) {
                    return a == b || a < b;
                }
                friend bool operator>(const _BasicEdge a, const _BasicEdge b) {
                    return !(a <= b);
                }
                friend bool operator>=(const _BasicEdge a, const _BasicEdge b) {
                    return !(a < b);
                }     
            };

            template<typename T>
            class _Edge : public _BasicEdge {
            protected:
                using _Self = _Edge<T>;
                T _w;
                _OUTPUT_FUNCTION(_Self)
            public:
                _Edge(int u, int v) : _BasicEdge(u, v), _w(T()) {
                    _output_function = default_function();
                }
                _Edge(int u, int v, T w) : _BasicEdge(u, v), _w(w) {
                    _output_function = default_function();
                }

                _SET_GET_VALUE(T, w)

                std::tuple<int, int, T> edge() const { return std::make_tuple(_u, _v, _w); }
                
                void default_output(std::ostream& os) const {
                    if (_swap_node) os << _v << " " << _u << " " << _w;
                    else os << _u << " " << _v << " " << _w;  
                }

                _OUTPUT_FUNCTION_SETTING(_Self)
            };

            template<>
            class _Edge<void> : public _BasicEdge {
            protected:
                using _Self = _Edge<void>;
                _OUTPUT_FUNCTION(_Self)
            public:
                _Edge(int u, int v) : _BasicEdge(u, v) {
                    _output_function = default_function();
                }

                std::tuple<int, int> edge() const { return std::make_tuple(_u, _v); }

                void default_output(std::ostream& os) const {
                    if (_swap_node) os << _v << " " << _u ;
                    else os << _u << " " << _v ;     
                }

                _OUTPUT_FUNCTION_SETTING(_Self)
            };
        } // namespace _basic
    } // namespace rand_graph
} // namespace generator

#endif // !_SGPCET_EDGE_H_