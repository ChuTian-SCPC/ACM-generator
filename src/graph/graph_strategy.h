#ifndef _SGPCET_GRAPH_STRATEGY_H_
#define _SGPCET_GRAPH_STRATEGY_H_

#ifndef _SGPCET_TREE_H_
#include "tree.h"
#endif // !_SGPCET_TREE_H_

namespace generator {
    namespace rand_graph {
        namespace basic {

            template <template <typename, typename> class GraphType, typename NodeType, typename EdgeType>
            class BasicGraphGen : public tools::_BasicGen<GraphType<NodeType, EdgeType>> {
            protected:
                std::map<_BasicEdge, bool> _e;
            public:
                BasicGraphGen(GraphType<NodeType, EdgeType>& context) : tools::_BasicGen<GraphType<NodeType, EdgeType>>(context) {}
                virtual void generate() override {
                    _msg::OutStream graph_log(false);
                    _msg::_defl.swap(graph_log);
                    __init();
                    __generate_graph(); 
                    __generate_nodes_weight();
                    _CONTEXT_GET_REF(edges)
                    shuffle(edges.begin(), edges.end());
                    _msg::_defl.swap(graph_log);
                };

            protected :

                virtual void __judge_upper_limit() {
                    _CONTEXT_GET(node_count)
                    _CONTEXT_GET(edge_count)
                    if (edge_count > _setting::edge_limit) {
                        _msg::__fail_msg(_msg::_defl,
                            tools::string_format("edge_count can't greater than edge_limit(%d), but found %d.",
                            _setting::edge_limit, edge_count));
                    }
                    if (!_CONTEXT_V(multiply_edge)) {  
                        long long limit = (long long) node_count * (long long) (node_count - 1) / 2;
                        if (_CONTEXT_V(direction)) limit *= 2;
                        if (_CONTEXT_V(self_loop)) limit += node_count;
                        if (edge_count > limit) {
                            _msg::__fail_msg(_msg::_defl, 
                            tools::string_format("edge_count must less than or equal to %lld, but found %d."
                            , limit, edge_count));
                        }
                    }
                    else {               
                        if (node_count == 1 && !_CONTEXT_V(self_loop) && edge_count > 0) {
                            _msg::__fail_msg(_msg::_defl, 
                                tools::string_format("number of edges must equal to 0, but found %d.",
                                edge_count));
                        }

                    }                        
                }

                virtual void __judge_lower_limit() {
                    _CONTEXT_GET(node_count)
                    _CONTEXT_GET(edge_count)
                    if (edge_count < 0) {
                        _msg::__fail_msg(_msg::_defl, "edge_count must be a non-negative integer.");
                    }
                    if (_CONTEXT_V(connect) && edge_count < node_count - 1) {
                        _msg::__fail_msg(_msg::_defl, 
                            tools::string_format("edge_count must greater than or equal to %d, but found %d.", 
                            node_count - 1, edge_count));
                    }
                }

                virtual void __judge_self_limit() {}

                void __judge_limits() {
                    __judge_upper_limit();
                    __judge_lower_limit();
                    __judge_self_limit();
                }
                
                virtual void __self_init(){};
                
                void __init() {
                    __self_init();
                    this->_context.check_gen_function();
                    __judge_limits();
                    _CONTEXT_V_REF(edges).clear();
                    __clear_nodes_weight();
                    if (!_CONTEXT_V(multiply_edge)) _e.clear();          
                    _CONTEXT_GET(node_indices)
                    _CONTEXT_GET(node_count)
                    if ((int)node_indices.size() != node_count) {
                        _msg::__warn_msg(_msg::_defl, 
                            tools::string_format("size of node_indices %d is not equal to node_count %d, use default node_indices.",
                            node_indices.size(), node_count));
                        this->_context.default_node_indices();
                    }
                }
                
                bool __judge_self_loop(int u, int v) {
                    return !_CONTEXT_V(self_loop) && u == v;
                }

                bool __judge_multiply_edge(int u, int v) {
                    if (_CONTEXT_V(multiply_edge)) return false;
                    if (_e[{u, v}]) return true;
                    return false;
                }

                void __add_edge_into_map(int u, int v) {
                    if (!_CONTEXT_V(multiply_edge)) {
                        _e[{u, v}] = true;
                        if (!_CONTEXT_V(direction)) _e[{v, u}] = true;
                    }
                }
                
                void __add_edge(_Edge<EdgeType> edge) {
                    int u = edge.u();
                    int v = edge.v();
                    __add_edge_into_map(u, v);
                    _CONTEXT_V_REF(edges).emplace_back(edge);
                }
                
                template<typename T = EdgeType, _NotHasT<T> = 0>
                void __add_edge(int u, int v) {
                    __add_edge(_Edge<void>(u, v));
                }

                template<typename T = EdgeType, _HasT<T> = 0>
                void __add_edge(int u, int v) {
                    EdgeType w = this->_context.edges_weight_function()();
                    __add_edge(_Edge<EdgeType>(u, v, w));
                }

                template<typename T = NodeType, _NotHasT<T> = 0>
                void __generate_nodes_weight() { return; }
                
                template<typename T = NodeType, _HasT<T> = 0>
                void __generate_nodes_weight() {
                    for (int i = 0; i < _CONTEXT_V(node_count) ; i++) {
                        NodeType w = this->_context.nodes_weight_function()();
                        this->_context.nodes_weight_ref().emplace_back(w);
                    }
                }

                template<typename T = EdgeType, _NotHasT<T> = 0>
                _Edge<void> __convert_edge(int u, int v) {
                    return _Edge<void>(u, v);
                }

                template<typename T = EdgeType, _HasT<T> = 0>
                _Edge<EdgeType> __convert_edge(int u, int v) {
                    EdgeType w = this->_context.edges_weight_function()();
                    return _Edge<EdgeType>(u, v, w);
                }

                virtual _Edge<EdgeType> __rand_edge() {
                    _CONTEXT_GET(node_count)
                    int u, v;
                    do {
                        u = rnd.next(node_count);
                        v = rnd.next(node_count);
                    } while (__judge_self_loop(u, v) || __judge_multiply_edge(u, v));
                    return this->__convert_edge(u, v);
                }

                virtual void __generate_connect() {
                    _CONTEXT_GET(node_count)
                    Tree<void, void> tree(node_count, 0);
                    tree.gen();
                    std::vector <_Edge<void>> edge = tree.edges();
                    for (auto e: edge) __add_edge(e.u(), e.v());
                }

                virtual void __generate_graph() {
                    int m = _CONTEXT_V(edge_count);
                    if (_CONTEXT_V(connect)) {
                        m -= _CONTEXT_V(node_count) - 1;
                        __generate_connect();
                    }
                    while (m--) {
                        __add_edge(__rand_edge());
                    }
                }

                template <typename T = NodeType, _NotHasT<T> = 0>
                void __clear_nodes_weight() { return; }

                template <typename T = NodeType, _HasT<T> = 0>
                void __clear_nodes_weight() {
                    this->_context.nodes_weight_ref().clear(); 
                }
            };

            class _GraphGenSwitch : public tools::_GenSwitch {
            public:
                void set_graph_generator(tools::_Gen* gen) {
                    __delete_generator();
                    _generator = gen;
                }
            };
        } // namespace basic
    } // namespace rand_graph
} // namespace generator

#endif // !_SGPCET_GRAPH_STRATEGY_H_
