#ifndef _SGPCET_GEN_TREE_H_
#define _SGPCET_GEN_TREE_H_

#ifndef _SGPCET_BASIC_TREE_GRAPH_H_
#include "basic_tree_graph.h"
#endif // !_SGPCET_BASIC_TREE_GRAPH_H_
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
            class _NoGenTree : public _BasicTree, public _RandomFunction<NodeType, EdgeType> {
            protected:
                using _Self =  _NoGenTree<NodeType,EdgeType>;
                _OUTPUT_FUNCTION(_Self)
                _DEF_GEN_FUNCTION
                std::vector<_Edge<EdgeType>> _edges;
                std::vector<_Node<NodeType>> _nodes_weight;          
            public:
                friend class _LinkImpl<NodeType, EdgeType>;
                friend class _TreeLinkImpl<NodeType, EdgeType>;
            public:
                template<typename T = NodeType, typename U = EdgeType, _IsBothWeight<T, U> = 0>
                _NoGenTree(int node_count = 1, int begin_node = 1, bool is_rooted = false, int root = 1,
                    NodeGenFunction nodes_weight_function = nullptr,
                    EdgeGenFunction edges_weight_function = nullptr) :
                    _BasicTree(node_count, begin_node, is_rooted, root, true, true),
                    _RandomFunction<NodeType, EdgeType>(nodes_weight_function, edges_weight_function)
                {
                    _output_function = default_function();
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsEdgeWeight<T, U> = 0>
                _NoGenTree(int node_count = 1, int begin_node = 1, bool is_rooted = false, int root = 1,
                    EdgeGenFunction edges_weight_function = nullptr) :
                    _BasicTree(node_count, begin_node, is_rooted, root, true, true),
                    _RandomFunction<NodeType, EdgeType>(nullptr, edges_weight_function)
                {
                    _output_function = default_function();
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsNodeWeight<T, U> = 0>
                _NoGenTree(int node_count = 1, int begin_node = 1, bool is_rooted = false, int root = 1,
                    NodeGenFunction nodes_weight_function = nullptr) :
                    _BasicTree(node_count, begin_node, is_rooted, root, true, true),
                    _RandomFunction<NodeType, EdgeType>(nodes_weight_function, nullptr)
                {
                    _output_function = default_function();
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsUnweight<T, U> = 0>
                _NoGenTree(int node_count = 1, int begin_node = 1, bool is_rooted = false, int root = 1) :
                    _BasicTree(node_count, begin_node, is_rooted, root, true, true),
                    _RandomFunction<NodeType, EdgeType>(nullptr, nullptr)
                {
                    _output_function = default_function();
                }

                virtual ~_NoGenTree() {}

                std::vector<_Edge<EdgeType>> edges() const { return __get_output_edges(); }
                std::vector<_Edge<EdgeType>>& edges_ref() { return _edges; }

                template<typename T = NodeType, _HasT<T> = 0>
                std::vector<_Node<NodeType>> nodes_weight() const { return _nodes_weight; }
                template<typename T = NodeType, _HasT<T> = 0>
                std::vector<_Node<NodeType>>& nodes_weight_ref() { return _nodes_weight; }
                
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
                    output_lines.push_back(__nodes_weight_format());
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

                void __reroot_set_check(int root) {
                    if (!_is_rooted) {
                        _msg::__warn_msg(_msg::_defl, "unrooted tree can't re-root.");
                        return;
                    }
                    if (root < 1 || root > _node_count) {
                        _msg::__warn_msg(_msg::_defl, 
                            tools::string_format("limit of the root is [1, %d], but found %d.", 
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

                template<typename T = NodeType, _NotHasT<T> = 0>
                std::string __nodes_weight_format() const {
                    return "";
                }

                template<typename T = NodeType, _HasT<T> = 0>
                std::string __nodes_weight_format() const {
                    return join(_nodes_weight);
                }
            };

            template<typename NodeType, typename EdgeType>
            class _GenTree : public _NoGenTree<NodeType, EdgeType>, public _TreeGenSwitch {
            protected:
                using _Self = _GenTree<NodeType,EdgeType>;
                _OUTPUT_FUNCTION(_Self)
                _DEF_GEN_FUNCTION
            public:
                using _TreeGenSwitch::set_tree_generator;
            public:
                template<typename T = NodeType, typename U = EdgeType, _IsBothWeight<T, U> = 0>
                _GenTree(int node_count = 1, int begin_node = 1, bool is_rooted = false, int root = 1,
                    NodeGenFunction nodes_weight_function = nullptr,
                    EdgeGenFunction edges_weight_function = nullptr) :
                    _NoGenTree<NodeType, EdgeType>(node_count, begin_node, is_rooted, root,
                        nodes_weight_function, edges_weight_function),
                    _TreeGenSwitch() {}
                
                template<typename T = NodeType, typename U = EdgeType, _IsEdgeWeight<T, U> = 0>
                _GenTree(int node_count = 1, int begin_node = 1, bool is_rooted = false, int root = 1,
                    EdgeGenFunction edges_weight_function = nullptr) :
                    _NoGenTree<void, EdgeType>(node_count, begin_node, is_rooted, root,
                        edges_weight_function),
                    _TreeGenSwitch() {}
                
                template<typename T = NodeType, typename U = EdgeType, _IsNodeWeight<T, U> = 0>
                _GenTree(int node_count = 1, int begin_node = 1, bool is_rooted = false, int root = 1,
                    NodeGenFunction nodes_weight_function = nullptr) :
                    _NoGenTree<NodeType, void>(node_count, begin_node, is_rooted, root,
                        nodes_weight_function),
                    _TreeGenSwitch() {}
                
                template<typename T = NodeType, typename U = EdgeType, _IsUnweight<T, U> = 0>
                _GenTree(int node_count = 1, int begin_node = 1, bool is_rooted = false, int root = 1) :
                    _NoGenTree<void, void>(node_count, begin_node, is_rooted, root),
                    _TreeGenSwitch() {}

                void gen() { 
                    this->_generator->generate(); 
                    if (this->_is_rooted) this->__reroot();
                }
            };

        } // namespace basic
    } // namespace rand_graph
} // namespace generator

#endif // !_SGPCET_GEN_TREE_H_
