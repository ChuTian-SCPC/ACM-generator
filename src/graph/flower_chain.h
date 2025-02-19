#ifndef _SGPCET_FLOWER_CHAIN_H_
#define _SGPCET_FLOWER_CHAIN_H_

#ifndef _SGPCET_FLOWER_H_
#include "flower.h"
#endif //!_SGPCET_FLOWER_H_
#ifndef _SGPCET_CHAIN_H_
#include "chain.h"
#endif //!_SGPCET_CHAIN_H_
#ifndef _SGPCET_LINK_H_
#include "link.h"
#endif //!_SGPCET_LINK_H_

namespace generator {
    namespace rand_graph {
        namespace basic {
            template<typename NodeType, typename EdgeType>
            class FlowerChain;

            template <typename NodeType, typename EdgeType>
            class FlowerChainGen : public BasicTreeGen<FlowerChain, NodeType, EdgeType> {
            protected:
                TreeLink<NodeType, EdgeType> _link;
            public:
                using Context = FlowerChain<NodeType, EdgeType>;
                FlowerChainGen(Context& tree) : BasicTreeGen<FlowerChain, NodeType, EdgeType>(tree), _link() {}
            
            protected:
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

                virtual void __self_init() override {
                    _CONTEXT_GET_REF(flower_size);
                    _CONTEXT_GET_REF(chain_size);
                    _CONTEXT_GET(node_count);
                    if (flower_size == -1) flower_size = rand_numeric::rand_int(0, node_count);
                    chain_size = node_count - flower_size;
                }
                
                virtual void __judge_self_limit() override {
                    _CONTEXT_GET(flower_size);
                    _CONTEXT_GET(chain_size);
                    if (flower_size < 0) 
                        _msg::__fail_msg(_msg::_defl, tools::string_format("Flower size must greater than or equal to 0, but found %d.", flower_size));
                    if (chain_size < 0) 
                        _msg::__fail_msg(_msg::_defl, tools::string_format("Chain size must greater than or equal to 0, but found %d.", chain_size));
                }

                virtual void __generate_tree() override {
                    _CONTEXT_GET(flower_size);
                    _CONTEXT_GET(chain_size);
                    Flower<NodeType, EdgeType> flower(flower_size);
                    Chain<NodeType, EdgeType> chain(chain_size);
                    __reset_nodes_weight_function(flower);
                    __reset_edges_weight_function(flower);
                    __reset_nodes_weight_function(chain);
                    __reset_edges_weight_function(chain);
                    flower.set_log_change(false);
                    chain.set_log_change(false);
                    _link.set_target(this->_context);
                    _link.add_source(flower);
                    _link.add_source(chain);
                    _link.gen();
                    __dump_result();
                }

                void __dump_result() {
                    _CONTEXT_V_REF(node_indices) = _link.node_indices();
                    _CONTEXT_V_REF(edges) = _link.edges_ref();
                }
            };

            template <typename NodeType, typename EdgeType>
            class FlowerChain : public _GenTree<NodeType, EdgeType> {
            protected:
                using _Self = FlowerChain<NodeType, EdgeType>;
                _OUTPUT_FUNCTION(_Self)
                _DEF_GEN_FUNCTION
                int _flower_size;
                int _chain_size;
            public:
                template<typename T = NodeType, typename U = EdgeType, _IsBothWeight<T, U> = 0>
                FlowerChain(int node = 1, int begin_node = 1, bool is_rooted = false, int root = 1, int flower_size = -1,
                    NodeGenFunction nodes_weight_function = nullptr,
                    EdgeGenFunction edges_weight_function = nullptr) :
                    _GenTree<NodeType, EdgeType>(node, begin_node, is_rooted, root,
                        nodes_weight_function, edges_weight_function),
                    _flower_size(flower_size)
                {
                    _TREE_GRAPH_DEFAULT
                }

                template<typename T = NodeType, typename U = EdgeType, _IsEdgeWeight<T, U> = 0>
                FlowerChain(int node = 1, int begin_node = 1, bool is_rooted = false, int root = 1, int flower_size = -1,
                    EdgeGenFunction edges_weight_function = nullptr) :
                    _GenTree<NodeType, EdgeType>(node, begin_node, is_rooted, root, edges_weight_function),
                    _flower_size(flower_size)
                {
                    _TREE_GRAPH_DEFAULT
                }

                template<typename T = NodeType, typename U = EdgeType, _IsNodeWeight<T, U> = 0>
                FlowerChain(int node = 1, int begin_node = 1, bool is_rooted = false, int root = 1, int flower_size = -1,
                    NodeGenFunction nodes_weight_function = nullptr) :
                    _GenTree<NodeType, EdgeType>(node, begin_node, is_rooted, root, nodes_weight_function),
                    _flower_size(flower_size)
                {
                    _TREE_GRAPH_DEFAULT
                }

                template<typename T = NodeType, typename U = EdgeType, _IsUnweight<T, U> = 0>
                FlowerChain(int node = 1, int begin_node = 1, bool is_rooted = false, int root = 1, int flower_size = -1) :
                    _GenTree<NodeType, EdgeType>(node, begin_node, is_rooted, root),
                    _flower_size(flower_size)
                {
                    _TREE_GRAPH_DEFAULT
                }

                _GET_VALUE(int, flower_size)
                _GET_VALUE(int, chain_size)

                void set_flower_size(int flower_size) {
                    _flower_size = flower_size;
                    _chain_size = this->_node_count - flower_size;
                }
                
                void set_chain_size(int chain_size) {
                    _chain_size = chain_size;
                    _flower_size = this->_node_count - _chain_size;
                }
                
                void set_flower_chain_size(int flower_size, int chain_size) {
                    _flower_size = flower_size;
                    _chain_size = chain_size;
                    this->_node_count = flower_size + chain_size;
                    this->__init_node_indices();
                } 

                _OUTPUT_FUNCTION_SETTING(_Self)
            protected:
                _DEFAULT_GEN_FUNC(FlowerChain)
            };
        } // namespace basic
    } // namespace rand_graph
} // namespace generator


#endif // !_SGPCET_FLOWER_CHAIN_H_
