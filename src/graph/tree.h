#ifndef _SGPCET_TREE_H_
#define _SGPCET_TREE_H_

#ifndef _SGPCET_BASIC_TREE_H_
#include "basic_tree.h"
#endif // !_SGPCET_BASIC_TREE_H_
#ifndef _SGPCET_GEN_FUNCTION_H_
#include "gen_function.h"
#endif // !_SGPCET_GEN_FUNCTION_H_
#ifndef _SGPCET_GEN_STRATEGY_H_
#include "gen_strategy.h"
#endif // !_SGPCET_GEN_STRATEGY_H_
#ifndef _SGPCET_LINK_FORWARD_H_
#include "link_forward.h"
#endif // !_SGPCET_LINK_FORWARD_H_
#ifndef _SGPCET_ARRAY_H_
#include "rand/array.h"
#endif // !_SGPCET_ARRAY_H_

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

            template<typename NodeType, typename EdgeType>
            class Tree : public _BasicTree, public _RandomFunction<NodeType, EdgeType> {
            protected:
                using _Self =  Tree<NodeType,EdgeType>;
                _OUTPUT_FUNCTION(_Self)
                _DEF_GEN_FUNCTION
                std::vector<_Edge<EdgeType>> _edges;
                std::vector<_Node<NodeType>> _nodes_weight; 
                TreeGen<NodeType, EdgeType>* _tree_generator;          
            public:
                friend class _LinkImpl<NodeType, EdgeType>;
                friend class _TreeLinkImpl<NodeType, EdgeType>;
            public:
                template<typename T = NodeType, typename U = EdgeType, _IsBothWeight<T, U> = 0>
                Tree(
                    int node_count = 1, int begin_node = 1, bool is_rooted = false, int root = 1,
                    NodeGenFunction nodes_weight_function = nullptr,
                    EdgeGenFunction edges_weight_function = nullptr,
                    _enum::TreeGenerator tree_generator = _enum::RandomFather) :
                    _BasicTree(node_count, begin_node, is_rooted, root, true, true),
                    _RandomFunction<NodeType, EdgeType>(nodes_weight_function, edges_weight_function),
                    _tree_generator(nullptr) 
                {
                    _output_function = default_function();
                    set_tree_generator(tree_generator);
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsEdgeWeight<T, U> = 0>
                Tree(
                    int node_count = 1, int begin_node = 1, bool is_rooted = false, int root = 1,
                    EdgeGenFunction edges_weight_function = nullptr,
                    _enum::TreeGenerator tree_generator = _enum::RandomFather) :
                    _BasicTree(node_count, begin_node, is_rooted, root, true, true),
                    _RandomFunction<NodeType, EdgeType>(nullptr, edges_weight_function),
                    _tree_generator(nullptr) 
                {
                    _output_function = default_function();
                    set_tree_generator(tree_generator);
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsNodeWeight<T, U> = 0>
                Tree(
                    int node_count = 1, int begin_node = 1, bool is_rooted = false, int root = 1,
                    NodeGenFunction nodes_weight_function = nullptr,
                    _enum::TreeGenerator tree_generator = _enum::RandomFather) :
                    _BasicTree(node_count, begin_node, is_rooted, root, true, true),
                    _RandomFunction<NodeType, EdgeType>(nodes_weight_function, nullptr),
                    _tree_generator(nullptr) 
                {
                    _output_function = default_function();
                    set_tree_generator(tree_generator);
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsUnweight<T, U> = 0>
                Tree(
                    int node_count = 1, int begin_node = 1, bool is_rooted = false, int root = 1,
                    _enum::TreeGenerator tree_generator = _enum::RandomFather) :
                    _BasicTree(node_count, begin_node, is_rooted, root, true, true),
                    _RandomFunction<NodeType, EdgeType>(nullptr, nullptr),
                    _tree_generator(nullptr) 
                {
                    _output_function = default_function();
                    set_tree_generator(tree_generator);
                }

                virtual ~Tree() {
                    __delete_tree_generator();
                }

                std::vector<_Edge<EdgeType>> edges() const { return __get_output_edges(); }
                std::vector<_Edge<EdgeType>>& edges_ref() { return _edges; }

                template<typename T = NodeType, _HasT<T> = 0>
                std::vector<_Node<NodeType>> nodes_weight() const { return _nodes_weight; }
                template<typename T = NodeType, _HasT<T> = 0>
                std::vector<_Node<NodeType>>& nodes_weight_ref() { return _nodes_weight; }

                void set_tree_generator(_enum::TreeGenerator tree_generator) {
                    if (tree_generator == _enum::RandomFather) use_random_father();
                    else use_pruefer();
                }
                void set_tree_generator(TreeGen<NodeType, EdgeType>* gen) {
                    __delete_tree_generator();
                    _tree_generator = gen;

                }
                void use_random_father() { 
                    __delete_tree_generator();
                    _tree_generator = new RandomFatherGen<NodeType, EdgeType>(*this); 
                }
                void use_pruefer() { 
                    __delete_tree_generator();
                    _tree_generator = new PrueferGen<NodeType, EdgeType>(*this);
                }

                void gen() { _tree_generator->generate(); }
                
                void reroot(int root) {
                    __reroot_set_check(root);
                    __reroot();
                }
                
                void default_output(std::ostream& os) const {
                    std::vector<int> first_line_vec;
                    if (_output_node_count) {
                        first_line_vec.push_back(_node_count);
                    }
                    if (_is_rooted && _output_root) {
                        first_line_vec.push_back(root());
                    }
                    std::vector<std::string> output_lines{join(first_line_vec)};
                    if (!std::is_void<NodeType>::value) {
                        output_lines.push_back(join(_nodes_weight));
                    }
                    std::vector<_Edge<EdgeType>> output_edges = __get_output_edges();
                    for (auto &edge : output_edges) {
                        if (_swap_node && rand_numeric::rand_bool()) {
                            edge.set_swap_node(true);
                        }
                    }
                    output_lines.push_back(join(output_edges, "\n"));

                    output_lines.erase(std::remove(output_lines.begin(), output_lines.end(), ""), output_lines.end());
                    os << join(output_lines, "\n");
                }

                _OUTPUT_FUNCTION_SETTING(_Self)
            protected:
                void __delete_tree_generator() {
                    if (_tree_generator) delete _tree_generator;
                }

                void __reroot_set_check(int root) {
                    if (!_is_rooted) {
                        _msg::__warn_msg(_msg::_defl, "unrooted tree can't re-root.");
                        return;
                    }
                    if (root < 1 || root > _node_count) {
                        _msg::__warn_msg(_msg::_defl, 
                            tools::string_format("restriction of the root is [1, %d], but found %d.", 
                            _node_count, root));
                        return;
                    }
                    if ((int)_edges.size() < _node_count - 1) {
                        _msg::__warn_msg(_msg::_defl, "should use gen() to generate tree first.");
                        return;
                    }
                    _root = root - 1;
                }

                void __reroot() {
                    std::vector<_Edge<EdgeType>> result;
                    std::vector<std::vector<_Edge<EdgeType>>> node_edges(_node_count);
                    for (auto edge : _edges) {
                        node_edges[edge.u()].emplace_back(edge);
                        node_edges[edge.v()].emplace_back(edge);
                    }
                    std::vector<int> visit(_node_count, 0);
                    std::queue<int> q;
                    q.push(_root);
                    while(!q.empty()) {
                        int u = q.front();
                        q.pop();
                        visit[u] = 1;
                        for (auto& edge : node_edges[u]) {
                            if (edge.u() != u) {
                                std::swap(edge.u_ref(), edge.v_ref());
                            }
                            int v = edge.v();
                            if (visit[v]) {
                                continue;
                            }
                            result.emplace_back(edge);
                            q.push(v);
                        }
                    }
                    shuffle(result.begin(), result.end());
                    _edges = result;       
                }

                template<typename T = EdgeType, _NotHasT<T> = 0>
                std::vector<_Edge<EdgeType>> __get_output_edges() const {
                    std::vector<_Edge<EdgeType>> output_edges;
                    for (const auto& edge : _edges) {
                        output_edges.emplace_back(_node_indices[edge.u()], _node_indices[edge.v()]);
                    }
                    return output_edges;
                }

                template<typename T = EdgeType, _HasT<T> = 0>
                std::vector<_Edge<EdgeType>> __get_output_edges() const {
                    std::vector<_Edge<EdgeType>> output_edges;
                    for (const auto& edge : _edges) {
                        output_edges.emplace_back(_node_indices[edge.u()], _node_indices[edge.v()], edge.w());
                    }
                    return output_edges;
                }
            };

        } // namespace basic
    } // namespace rand_graph
} // namespace generator

#endif // !_SGPCET_TREE_H_
