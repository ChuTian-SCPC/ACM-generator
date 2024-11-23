#ifndef _SGPCET_FLOWER_H_
#define _SGPCET_FLOWER_H_

#ifndef _SGPCET_TREE_H_
#include "tree.h"
#endif // !_SGPCET_TREE_H_

namespace generator {
    namespace rand_graph {
        namespace basic {
            template<typename NodeType, typename EdgeType>
            class Flower;

            template <typename NodeType, typename EdgeType>
            class FlowerGen : public BasicRandomFatherGen<Flower, NodeType, EdgeType> {
            protected:
                using Context = Flower<NodeType, EdgeType>;
            public:
                FlowerGen(Context& tree) : BasicRandomFatherGen<Flower, NodeType, EdgeType>(tree) {}
            protected:
                virtual void __random_father() override {
                    for (int i = 1; i < this->_context.node_count(); i++) {
                        this->__add_edge(this->_rank[0], this->_rank[i]);
                    }                     
                }
            };

            template <typename NodeType, typename EdgeType>
            class Flower : public _GenTree<NodeType, EdgeType> {
            protected:
                using _Self =  Flower<NodeType,EdgeType>;
                _OUTPUT_FUNCTION(_Self)
                _DEF_GEN_FUNCTION
            public:
                template<typename T = NodeType, typename U = EdgeType, _IsBothWeight<T, U> = 0>
                Flower(int node = 1, int begin_node = 1, bool is_rooted = false, int root = 1,
                    NodeGenFunction nodes_weight_function = nullptr,
                    EdgeGenFunction edges_weight_function = nullptr) :
                    _GenTree<NodeType, EdgeType>(node, begin_node, is_rooted, root,
                        nodes_weight_function, edges_weight_function) 
                {
                    _TREE_GRAPH_DEFAULT       
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsEdgeWeight<T, U> = 0>
                Flower(int node = 1, int begin_node = 1, bool is_rooted = false, int root = 1,
                    EdgeGenFunction edges_weight_function = nullptr) :
                    _GenTree<NodeType, EdgeType>(node, begin_node, is_rooted, root, edges_weight_function)
                {
                    _TREE_GRAPH_DEFAULT        
                }

                template<typename T = NodeType, typename U = EdgeType, _IsNodeWeight<T, U> = 0>
                Flower(int node = 1, int begin_node = 1, bool is_rooted = false, int root = 1,
                    NodeGenFunction nodes_weight_function = nullptr) :
                    _GenTree<NodeType, EdgeType>(node, begin_node, is_rooted, root, nodes_weight_function) 
                {
                    _TREE_GRAPH_DEFAULT        
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsUnweight<T, U> = 0>
                Flower(int node = 1, int begin_node = 1, bool is_rooted = false, int root = 1) :
                    _GenTree<NodeType, EdgeType>(node, begin_node, is_rooted, root) 
                {
                    _TREE_GRAPH_DEFAULT       
                }  
                _OUTPUT_FUNCTION_SETTING(_Self)
            protected:
                _DEFAULT_GEN_FUNC(Flower)
            };   
        } // namespace basic
    } // namespace rand_graph
} // namespace generator

#endif // !_SGPCET_FLOWER_H_
