#ifndef _SGPCET_GEN_STRATEGY_H_
#define _SGPCET_GEN_STRATEGY_H_

#ifndef _SGPCET_COMMON_H_
#include "basic/common.h"
#endif // !_SGPCET_COMMON_H_
#ifndef _SGPCET_MACRO_H_
#include "basic/macro.h"
#endif // !_SGPCET_MACRO_H_
#ifndef _SGPCET_LOGGER_H_
#include "log/logger.h"
#endif // !_SGPCET_LOGGER_H_
#ifndef _SGPCET_NODE_H_
#include "node.h"
#endif // !_SGPCET_NODE_H_
#ifndef _SGPCET_EDGE_H_
#include "edge.h"
#endif // !_SGPCET_NODE_H_

namespace generator {
    namespace rand_graph {
        namespace basic {
            
            class Gen {
            public:
                Gen(){}
                virtual void generate() {
                    _msg::__fail_msg(_msg::_defl, "unsupport generator.");
                };
            };

            template <typename T>
            class BasicGen : public Gen {
            protected:
                T& _context;
            public:
                BasicGen(T& context) : _context(context) {}
            };

            template <template <typename, typename> class Tree, typename NodeType, typename EdgeType>
            class BasicTreeGen : public BasicGen<Tree<NodeType, EdgeType>> {
            public:
                BasicTreeGen(Tree<NodeType, EdgeType>& context) : BasicGen<Tree<NodeType, EdgeType>>(context) {}
                virtual void generate() override {
                    _msg::OutStream tree_log(false);
                    _msg::_defl.swap(tree_log);
                    __init();
                    __generate_tree(); 
                    __generate_nodes_weight();
                    _CONTEXT_GET_REF(auto, edges)
                    shuffle(edges.begin(), edges.end());
                    _msg::_defl.swap(tree_log);
                };
            protected:
                virtual void __generate_tree() {
                    _msg::__fail_msg(_msg::_defl, "unsupport tree generator.");
                }

                void __add_edge(_Edge<EdgeType> edge) {
                    this->_context.edges_ref().emplace_back(edge);
                }
                
                template<typename T = EdgeType, _NotHasT<T> = 0>
                void __add_edge(int u, int v) {
                    __add_edge(_Edge<void>(u, v));

                }

                template<typename T = EdgeType, _HasT<T> = 0>
                void __add_edge(int u, int v, int w) {
                    __add_edge(_Edge<EdgeType>(u, v, w));
                }
                
                template<typename T = EdgeType, _HasT<T> = 0>
                void __add_edge(int u, int v) {
                    EdgeType w = this->_context.edges_weight_function()();
                    __add_edge(u, v, w);
                }

                void __judge_comman_limit() {
                    _CONTEXT_GET(int, node_count)
                    if (node_count <= 0) {
                        _msg::__fail_msg(_msg::_defl, 
                            tools::string_format("number of nodes must be a positive integer, but found %d.", 
                            node_count));
                    }

                    if (_CONTEXT_V(is_rooted)) {
                        _CONTEXT_GET_REF(int, root)
                        if (root < 0 || root >= node_count) {
                            _msg::__fail_msg(_msg::_defl,
                                tools::string_format("restriction of the root is [1, %d], but found %d.", 
                                node_count, root + 1));                           
                        }
                    }
                }

                virtual void __judge_self_limit() {}

                void __judge_limits() {
                    __judge_comman_limit();
                    __judge_self_limit();
                }
                
                virtual void __self_init(){};
                
                void __init() {
                    __self_init();
                    this->_context.check_gen_function();
                    __judge_limits();
                    this->_context.edges_ref().clear();
                    __clear_nodes_weight();    
                    _CONTEXT_GET(auto, node_indices)
                    _CONTEXT_GET(int, node_count)
                    if ((int)node_indices.size() != node_count) {
                        _msg::__warn_msg(_msg::_defl, 
                            tools::string_format("size of node_indices %d is not equal to node_count %d, use default node_indices.",
                            node_indices.size(), node_count));
                        this->_context.default_node_indices();
                    }
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

                template <typename T = NodeType, _NotHasT<T> = 0>
                void __clear_nodes_weight() { return; }

                template <typename T = NodeType, _HasT<T> = 0>
                void __clear_nodes_weight() {
                    this->_context.nodes_weight_ref().clear(); 
                }
            };

            class _GenSwitch {
            protected:
                Gen* _generator;
            public:
                _GenSwitch() : _generator(nullptr) {}
                virtual ~_GenSwitch() { __delete_generator(); }

                void gen() { _generator->generate(); }
            protected:
                void __delete_generator() {
                    if (_generator) delete _generator;
                }
            };

            class _TreeGenSwitch : public _GenSwitch {
            public:
                void set_tree_generator(Gen* gen) {
                    __delete_generator();
                    _generator = gen;
                }
            };

            class _GraphGenSwitch : public _GenSwitch {
            public:
                void set_graph_generator(Gen* gen) {
                    __delete_generator();
                    _generator = gen;
                }
            };
        } // namespace basic
    } // namespace rand_graph
} // namespace generator

#endif // !_SGPCET_GEN_STRATEGY_H_
