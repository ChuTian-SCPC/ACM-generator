#ifndef _SGPCET_TREE_H_
#define _SGPCET_TREE_H_

#ifndef _SGPCET_GEN_TREE_H_
#include "gen_tree.h"
#endif // !_SGPCET_GEN_TREE_H_
#ifndef _SGPCET_ENUM_H_
#include "basic/enum.h"
#endif // !_SGPCET_ENUM_H_


namespace generator {
    namespace rand_graph {
        namespace basic {
            template<typename NodeType, typename EdgeType>
            class Tree;

            template <typename NodeType, typename EdgeType>
            class TreeGen : public BasicTreeGen<Tree, NodeType, EdgeType> {
            public:
                using Context = Tree<NodeType, EdgeType>;
                TreeGen(Context& tree) : BasicTreeGen<Tree, NodeType, EdgeType>(tree) {}
            };

            template <typename NodeType, typename EdgeType>
            class RandomFatherGen : public BasicRandomFatherGen<Tree, NodeType, EdgeType> {
            protected:
                using Context = Tree<NodeType, EdgeType>;        
            public:
                RandomFatherGen(Context& tree) : BasicRandomFatherGen<Tree, NodeType, EdgeType>(tree) {}
            };

            template <typename NodeType, typename EdgeType>
            class PrueferGen : public BasicPrueferGen<Tree, NodeType, EdgeType> {
            protected:
                using Context = Tree<NodeType, EdgeType>;
            public:
                PrueferGen(Context& tree) : BasicPrueferGen<Tree, NodeType, EdgeType>(tree) {}
            };

            template <typename NodeType, typename EdgeType>
            class Tree : public _GenTree<NodeType, EdgeType> {
            protected:
                using _Self =  Tree<NodeType,EdgeType>;
                _OUTPUT_FUNCTION(_Self)
                _DEF_GEN_FUNCTION
            public:
                using _GenTree<NodeType, EdgeType>::set_tree_generator;
            public:
                template<typename T = NodeType, typename U = EdgeType, _IsBothWeight<T, U> = 0>
                Tree(
                    int node_count = 1, int begin_node = 1, bool is_rooted = false, int root = 1,
                    NodeGenFunction nodes_weight_function = nullptr,
                    EdgeGenFunction edges_weight_function = nullptr,
                    _enum::TreeGenerator tree_generator = _enum::RandomFather) :
                    _GenTree<NodeType, EdgeType>(node_count, begin_node, is_rooted, root,
                        nodes_weight_function, edges_weight_function)
                {
                    _DEFAULT_OUTPUT
                    set_tree_generator(tree_generator);
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsEdgeWeight<T, U> = 0>
                Tree(
                    int node_count = 1, int begin_node = 1, bool is_rooted = false, int root = 1,
                    EdgeGenFunction edges_weight_function = nullptr,
                    _enum::TreeGenerator tree_generator = _enum::RandomFather) :
                    _GenTree<void, EdgeType>(node_count, begin_node, is_rooted, root, edges_weight_function)
                {
                    _DEFAULT_OUTPUT
                    set_tree_generator(tree_generator);
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsNodeWeight<T, U> = 0>
                Tree(
                    int node_count = 1, int begin_node = 1, bool is_rooted = false, int root = 1,
                    NodeGenFunction nodes_weight_function = nullptr,
                    _enum::TreeGenerator tree_generator = _enum::RandomFather) :
                    _GenTree<NodeType, void>(node_count, begin_node, is_rooted, root, nodes_weight_function)
                {
                    _DEFAULT_OUTPUT
                    set_tree_generator(tree_generator);
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsUnweight<T, U> = 0>
                Tree(
                    int node_count = 1, int begin_node = 1, bool is_rooted = false, int root = 1,
                    _enum::TreeGenerator tree_generator = _enum::RandomFather) :
                    _GenTree<void, void>(node_count, begin_node, is_rooted, root)
                {
                    _DEFAULT_OUTPUT
                    set_tree_generator(tree_generator);
                }

                void set_tree_generator(_enum::TreeGenerator tree_generator) {
                    if (tree_generator == _enum::RandomFather) use_random_father();
                    else use_pruefer(); 
                }
                void use_random_father() {
                    this->__delete_generator();
                    this->_generator = new RandomFatherGen<NodeType, EdgeType>(*this);
                }
                void use_pruefer() {
                    this->__delete_generator();
                    this->_generator = new PrueferGen<NodeType, EdgeType>(*this);
                }
            };

        } // namespace basic
    } // namespace rand_graph
} // namespace generator

#endif // !_SGPCET_TREE_H_
