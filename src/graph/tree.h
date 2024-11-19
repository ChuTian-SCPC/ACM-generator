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
            class RandomFatherGen : public TreeGen<NodeType, EdgeType> {
            protected:
                using Context = Tree<NodeType, EdgeType>;
                std::vector<int> _rank;
                
            public:
                RandomFatherGen(Context& tree) : TreeGen<NodeType, EdgeType>(tree) {}
            protected:
                virtual void __generate_tree() override {
                    __init_rank();
                    __random_father();
                }

                void __init_rank() {
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
                void __random_father() {
                    int node_count = this->_context.node_count();
                    for (int i = 1; i < node_count; i++) {
                        int f = rnd.next(i);
                        this->__add_edge(_rank[f], _rank[i]);
                    }
                }
            };

            template <typename NodeType, typename EdgeType>
            class PrueferGen : public TreeGen<NodeType, EdgeType> {
            protected:
                using Tree = Tree<NodeType, EdgeType>;
            public:
                PrueferGen(Tree& tree) : TreeGen<NodeType, EdgeType>(tree) {}
            protected:
                virtual void __generate_tree() override {
                    _CONTEXT_GET(int, node_count)
                    std::vector<int> times = rand_array::rand_sum(node_count, node_count - 2, 0);
                    std::vector<int> pruefer = rand_array::shuffle_index(times);
                    __pruefer_decode(pruefer);
                }
                void __pruefer_decode(std::vector<int> pruefer) {
                    _CONTEXT_GET(int, node_count)
                    _CONTEXT_GET(bool, is_rooted)
                    if (node_count == 2) {
                        int u = is_rooted ? _CONTEXT_V_REF(root) : 0;
                        this->__add_edge(u, 1 ^ u);
                        return;
                    }

                    if (is_rooted) {
                        _CONTEXT_GET_REF(int, root)
                        int n = pruefer.size();
                        bool exist = false;
                        for (int i = 0; i < n; i++) {
                            if (pruefer[i] == root) {
                                std::swap(pruefer[i], pruefer[n - 1]);
                                exist = true;
                                break;
                            }
                        }
                        if (!exist) {
                            pruefer[n - 1] = root;
                        }
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
                    set_tree_generator(tree_generator);
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsEdgeWeight<T, U> = 0>
                Tree(
                    int node_count = 1, int begin_node = 1, bool is_rooted = false, int root = 1,
                    EdgeGenFunction edges_weight_function = nullptr,
                    _enum::TreeGenerator tree_generator = _enum::RandomFather) :
                    _GenTree<void, EdgeType>(node_count, begin_node, is_rooted, root, edges_weight_function)
                {
                    set_tree_generator(tree_generator);
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsNodeWeight<T, U> = 0>
                Tree(
                    int node_count = 1, int begin_node = 1, bool is_rooted = false, int root = 1,
                    NodeGenFunction nodes_weight_function = nullptr,
                    _enum::TreeGenerator tree_generator = _enum::RandomFather) :
                    _GenTree<NodeType, void>(node_count, begin_node, is_rooted, root, nodes_weight_function)
                {
                    set_tree_generator(tree_generator);
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsUnweight<T, U> = 0>
                Tree(
                    int node_count = 1, int begin_node = 1, bool is_rooted = false, int root = 1,
                    _enum::TreeGenerator tree_generator = _enum::RandomFather) :
                    _GenTree<void, void>(node_count, begin_node, is_rooted, root)
                {
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
