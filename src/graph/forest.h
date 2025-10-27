#ifndef _SGPCET_FOREST_H_
#define _SGPCET_FOREST_H_

#ifndef _SGPCET_LINK_H_
#include "link.h"
#endif // !_SGPCET_LINK_H_

namespace generator {
    namespace rand_graph {
        namespace basic {
            template<typename NodeType, typename EdgeType>
            class Forest;

            template <typename NodeType, typename EdgeType>
            class ForestGen : public BasicGraphGen<Forest, NodeType, EdgeType> {
            protected:
                Link<NodeType, EdgeType> _link;
            public:
                using Context = Forest<NodeType, EdgeType>;
                ForestGen(Context& tree) : BasicGraphGen<Forest, NodeType, EdgeType>(tree), _link() {}
            protected:
                virtual void __judge_upper_limit() override {
                    _CONTEXT_GET(edge_count)
                    _CONTEXT_GET(node_count)
                    if (edge_count > node_count - 1) {
                        _msg::__fail_msg(_msg::_defl, tools::string_format("number of edges must less than %d.", node_count - 1));
                    }
                }

                void __reset_node_edge_count() {
                    _CONTEXT_GET(trees_size)
                    _CONTEXT_GET(node_count)
                    _CONTEXT_GET(edge_count)
                    int count = 0;
                    for (int tree_size : trees_size) {
                        count += tree_size;
                    }
                    if (count != node_count) {
                        _msg::__warn_msg(_msg::_defl, tools::string_format(
                            "node_count will be changed because the sum of Trees' size %d is not equal to node count %d.",
                            count, node_count));
                        this->_context.set_node_count(count);
                    }
                    if (count - (int)trees_size.size() != edge_count) {
                        _msg::__warn_msg(_msg::_defl, tools::string_format(
                            "edge_count will be changed because the sum of Trees' edges %d is not equal to edge count %d.",
                            count - (int)trees_size.size(), edge_count));
                        this->_context.set_edge_count(count - trees_size.size());                       
                    }
                }

                template<template<typename, typename> class TG, typename T = NodeType, _HasT<T> = 0>
                void __reset_nodes_weight_function(TG<NodeType, EdgeType>& source) {
                    auto func = this->_context.nodes_weight_function();
                    source.set_nodes_weight_function(func);
                }
                
                template<template<typename, typename> class TG, typename T = NodeType, _NotHasT<T> = 0>
                void __reset_nodes_weight_function(TG<NodeType, EdgeType>&) {
                    return;
                }
                
                template<template<typename, typename> class TG, typename T = EdgeType, _HasT<T> = 0>
                void __reset_edges_weight_function(TG<NodeType, EdgeType>& source) {
                    auto func = this->_context.edges_weight_function();
                    source.set_edges_weight_function(func);
                }
                
                template<template<typename, typename> class TG, typename T = EdgeType, _NotHasT<T> = 0>
                void __reset_edges_weight_function(TG<NodeType, EdgeType>&) {
                    return;
                }

                void __generate_trees_size() {
                    _CONTEXT_GET_REF(trees_size)
                    _CONTEXT_GET(node_count)
                    _CONTEXT_GET(edge_count)
                    int tree_count = node_count - edge_count;
                    this->_context.set_trees_size(rand_array::rand_sum(tree_count, node_count, 1));
                }

                virtual void __self_init() override {
                    if (_CONTEXT_V(trees_size).empty()) __generate_trees_size();
                    __reset_node_edge_count();
                    __init_connect();                    
                }

                virtual void __generate_graph() override {
                    _link.set_target(this->_context);                        
                    Tree<NodeType, EdgeType> tree;
                    __reset_nodes_weight_function(tree);
                    __reset_edges_weight_function(tree);
                    for (int tree_size : _CONTEXT_V(trees_size)) {
                        tree.set_node_count(tree_size);
                        tree.gen();
                        _link.add_source(tree);
                    }
                    _link.gen();
                    __dump_result();
                }

                void __init_connect() {
                    _CONTEXT_GET_REF(connect)
                    _CONTEXT_GET(node_count)
                    _CONTEXT_GET(edge_count)
                    connect = edge_count == node_count - 1;
                }

                void __dump_result() {
                    _CONTEXT_V_REF(node_indices) = _link.node_indices();
                    _CONTEXT_V_REF(edges) = _link.edges_ref();
                }
            };

            template <typename NodeType, typename EdgeType>
            class Forest : public _GenGraph<NodeType, EdgeType> {
            protected:
                using _Self = Forest<NodeType, EdgeType>;
                _OUTPUT_FUNCTION(_Self)
                _DEF_GEN_FUNCTION
                std::vector<int> _trees_size;
            public:
                template<typename T = NodeType, typename U = EdgeType, _IsBothWeight<T, U> = 0>
                Forest(
                    int node_count = 1, int edge_count = 0, int begin_node = 1, 
                    NodeGenFunction nodes_weight_function = nullptr,
                    EdgeGenFunction edges_weight_function = nullptr) :
                    _GenGraph<NodeType, EdgeType>(node_count, edge_count, begin_node,
                        false, false, false, edge_count == node_count - 1, true,
                        nodes_weight_function, edges_weight_function)
                {
                    _TREE_GRAPH_DEFAULT 
                }

                template<typename T = NodeType, typename U = EdgeType, _IsNodeWeight<T, U> = 0>
                Forest(
                    int node_count = 1, int edge_count = 0, int begin_node = 1, 
                    NodeGenFunction nodes_weight_function = nullptr) :
                    _GenGraph<NodeType, void>(node_count, edge_count, begin_node,
                        false, false, false, edge_count == node_count - 1, true,
                        nodes_weight_function)
                {
                    _TREE_GRAPH_DEFAULT 
                }

                template<typename T = NodeType, typename U = EdgeType, _IsEdgeWeight<T, U> = 0>
                Forest(
                    int node_count = 1, int edge_count = 0, int begin_node = 1, 
                    EdgeGenFunction edges_weight_function = nullptr) :
                    _GenGraph<void, EdgeType>(node_count, edge_count, begin_node,
                        false, false, false, edge_count == node_count - 1, true,
                        edges_weight_function)
                {
                    _TREE_GRAPH_DEFAULT 
                }

                template<typename T = NodeType, typename U = EdgeType, _IsUnweight<T, U> = 0>
                Forest(int node_count = 1, int edge_count = 0, int begin_node = 1) :
                    _GenGraph<void, void>(node_count, edge_count, begin_node,
                        false, false, false, edge_count == node_count - 1, true)
                {
                    _TREE_GRAPH_DEFAULT 
                }

                _GET_VALUE(std::vector<int>, trees_size)
                void add_tree_size(int tree_size) {
                    if (tree_size > 0) _trees_size.emplace_back(tree_size);
                    else _msg::__warn_msg(_msg::_defl, tools::string_format("Tree size must greater than 0, but found %d.", tree_size));
                }
                void set_trees_size(std::vector<int> trees_size) {
                    _trees_size.clear();
                    for (int tree_size : trees_size) add_tree_size(tree_size);
                }

                _DISABLE_DIRECTION
                _DISABLE_MULTIPLY_EDGE
                _DISABLE_SELF_LOOP
                _DISABLE_CONNECT
                _OUTPUT_FUNCTION_SETTING(_Self)
            
            protected:
                _DEFAULT_GRAPH_GEN_FUNC(Forest)
            };

        } // namespace basic
    } // namespace rand_graph
} // namespace generator

#endif // !_SGPCET_FOREST_H_
