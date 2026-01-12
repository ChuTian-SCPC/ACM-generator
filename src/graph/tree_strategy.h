#ifndef _SGPCET_TREE_STRATEGY_H_
#define _SGPCET_TREE_STRATEGY_H_

#ifndef _SGPCET_COMMON_H_
#include "basic/common.h"
#endif // !_SGPCET_COMMON_H_
#ifndef _SGPCET_MACRO_H_
#include "basic/macro.h"
#endif // !_SGPCET_MACRO_H_
#ifndef _SGPCET_GEN_STRATEGY_H_
#include "basic/gen_strategy.h"
#endif // !_SGPCET_GEN_STRATEGY_H_
#ifndef _SGPCET_NODE_H_
#include "node.h"
#endif // !_SGPCET_NODE_H_
#ifndef _SGPCET_EDGE_H_
#include "edge.h"
#endif // !_SGPCET_EDGE_H_
#ifndef _SGPCET_WEIGHT_TYPE_H_
#include "weight_type.h"
#endif // !_SGPCET_WEIGHT_TYPE_H_
#ifndef _SGPCET_ARRAY_H_
#include "rand/array.h"
#endif // !_SGPCET_ARRAY_H_

namespace generator {
    namespace rand_graph {
        namespace basic {

            template <template <typename, typename> class TreeType, typename NodeType, typename EdgeType>
            class BasicTreeGen : public tools::_BasicGen<TreeType<NodeType, EdgeType>> {
            public:
                BasicTreeGen(TreeType<NodeType, EdgeType>& context) : tools::_BasicGen<TreeType<NodeType, EdgeType>>(context) {}
                virtual void generate() override {                      
                    __init();
                    __generate_tree(); 
                    __generate_nodes_weight();
                    _CONTEXT_GET_REF(edges)
                    shuffle(edges.begin(), edges.end());
                };
            protected:
                virtual void __generate_tree() {
                    _msg::__fail_msg(_msg::_defl, "unsupport tree generator.");
                }

                void __add_edge(_Edge<EdgeType> edge) {
                    _CONTEXT_V_REF(edges).emplace_back(edge);
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
                    _CONTEXT_GET(node_count)
                    if (node_count <= 0) {
                        _msg::__fail_msg(_msg::_defl, 
                            tools::string_format("node_count must be a positive integer, but found %d.", 
                            node_count));
                    }

                    if (node_count > _setting::node_limit) {
                        _msg::__fail_msg(_msg::_defl,
                            tools::string_format("node_count can't greater than node_limit(%d), but found.",
                            _setting::node_limit, node_count));
                    }

                    if (_CONTEXT_V(is_rooted)) {
                        _CONTEXT_GET_REF(root)
                        if (root < 0 || root >= node_count) {
                            _msg::__fail_msg(_msg::_defl,
                                tools::string_format("limit of the root is [1, %d], but found %d.", 
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
                    _CONTEXT_V_REF(edges).clear();
                    __clear_nodes_weight();    
                    _CONTEXT_GET(node_indices)
                    _CONTEXT_GET(node_count)
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

            template <template <typename, typename> class TreeType, typename NodeType, typename EdgeType>
            class BasicRandomFatherGen : public BasicTreeGen<TreeType, NodeType, EdgeType> {
            protected:
                using Context = TreeType<NodeType, EdgeType>;
                std::vector<int> _rank;
                
            public:
                BasicRandomFatherGen(Context& tree) : BasicTreeGen<TreeType, NodeType, EdgeType>(tree) {}
            protected:
                virtual void __generate_tree() override {
                    __init_rank();
                    __random_father();
                }

                virtual void __init_rank() {
                    int node_count = this->_context.node_count();
                    _rank = rand_array::rand_p(node_count);
                    if (this->_context.is_rooted()) {
                        for (int i = 1; i < node_count; i++) {
                            if (_rank[i] == this->_context.root_ref()) {
                                std::swap(_rank[0], _rank[i]);
                                break;
                            }
                        } 
                    }
                }
                virtual void __random_father() {
                    int node_count = this->_context.node_count();
                    for (int i = 1; i < node_count; i++) {
                        int f = rnd.next(i);
                        this->__add_edge(_rank[f], _rank[i]);
                    }
                }
            };

            template <template <typename, typename> class TreeType, typename NodeType, typename EdgeType>
            class BasicPrueferGen : public BasicTreeGen<TreeType, NodeType, EdgeType> {
            protected:
                using Context = TreeType<NodeType, EdgeType>;
            public:
                BasicPrueferGen(Context& tree) : BasicTreeGen<TreeType, NodeType, EdgeType>(tree) {}
            protected:
                virtual void __generate_tree() override {
                    _CONTEXT_GET(node_count)
                    if (node_count == 1) return;
                    __generate_pruefer();
                }

                virtual void __generate_pruefer() {
                    _CONTEXT_GET(node_count);
                    std::vector<int> times = rand_array::rand_sum(node_count, node_count - 2, 0);
                    std::vector<int> pruefer = rand_array::shuffle_index(times);
                    __pruefer_decode(pruefer);                    
                }
                void __pruefer_decode(std::vector<int> pruefer) {
                    _CONTEXT_GET(node_count)
                    _CONTEXT_GET(is_rooted)
                    if (node_count == 2) {
                        int u = is_rooted ? _CONTEXT_V_REF(root) : 0;
                        this->__add_edge(u, 1 ^ u);
                        return;
                    }

                    std::vector<int> degree(node_count, 1);
                    for (auto x: pruefer) {
                        degree[x]++;
                    }
                    int ptr = 0;
                    while (degree[ptr] != 1) {
                        ptr++;
                    }
                    int leaf = ptr;
                    for (auto u: pruefer) {
                        this->__add_edge(u, leaf);
                        degree[u]--;
                        if (degree[u] == 1 && u < ptr) {
                            leaf = u;
                        } else {
                            do {
                                ptr++;
                            } while (degree[ptr] != 1);
                            leaf = ptr;
                        }
                    }
                    int u = leaf;
                    int v = node_count - 1;
                    if (is_rooted && v == _CONTEXT_V_REF(root)) {
                        std::swap(u, v);
                    }
                    this->__add_edge(u, v);
                }
            };

            class _TreeGenSwitch : public tools::_GenSwitch {
            public:
                void set_tree_generator(tools::_Gen* gen) {
                    __delete_generator();
                    _generator = gen;
                }
            };

        } // namespace basic
    } // namespace rand_graph
} // namespace generator

#endif // !_SGPCET_TREE_STRATEGY_H_
