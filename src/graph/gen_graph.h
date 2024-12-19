#ifndef _SGPCET_GEN_GRAPH_H_
#define _SGPCET_GEN_GRAPH_H_

#ifndef _SGPCET_BASIC_TREE_GRAPH_H_
#include "basic_tree_graph.h"
#endif // !_SGPCET_BASIC_TREE_GRAPH_H_
#ifndef _SGPCET_GEN_FUNCTION_H_
#include "gen_function.h"
#endif // !_SGPCET_GEN_FUNCTION_H_
#ifndef _SGPCET_GRAPH_STRATEGY_H_
#include "graph_strategy.h"
#endif // !_SGPCET_GRAPH_STRATEGY_H_
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
            class _RandomFuncGraph : public _BasicGraph, public _RandomFunction<NodeType, EdgeType> {
            protected:
                using _Self =  _RandomFuncGraph<NodeType,EdgeType>;
                _OUTPUT_FUNCTION(_Self)
                _DEF_GEN_FUNCTION
                std::vector<_Edge<EdgeType>> _edges;
                std::vector<_Node<NodeType>> _nodes_weight;          
            public:
                friend class _LinkImpl<NodeType, EdgeType>;
                friend class _TreeLinkImpl<NodeType, EdgeType>;
            public:
                template<typename T = NodeType, typename U = EdgeType, _IsBothWeight<T, U> = 0>
                _RandomFuncGraph(int node_count, int edge_count, int begin_node,
                    bool direction, bool multiply_edge, bool self_loop, bool connect, bool swap_node,
                    NodeGenFunction nodes_weight_function,
                    EdgeGenFunction edges_weight_function) :
                    _BasicGraph(node_count, edge_count, begin_node,
                        direction, multiply_edge, self_loop, connect, swap_node, true, true),
                    _RandomFunction<NodeType, EdgeType>(nodes_weight_function, edges_weight_function)
                {
                    _output_function = default_function();
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsEdgeWeight<T, U> = 0>
                _RandomFuncGraph(int node_count, int edge_count, int begin_node,
                    bool direction, bool multiply_edge, bool self_loop, bool connect, bool swap_node,
                    EdgeGenFunction edges_weight_function) :
                    _BasicGraph(node_count, edge_count, begin_node,
                        direction, multiply_edge, self_loop, connect, swap_node, true, true),
                    _RandomFunction<NodeType, EdgeType>(nullptr, edges_weight_function)
                {
                    _output_function = default_function();
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsNodeWeight<T, U> = 0>
                _RandomFuncGraph(int node_count, int edge_count, int begin_node,
                    bool direction, bool multiply_edge, bool self_loop, bool connect, bool swap_node,
                    NodeGenFunction nodes_weight_function) :
                    _BasicGraph(node_count, edge_count, begin_node,
                        direction, multiply_edge, self_loop, connect, swap_node, true, true),
                    _RandomFunction<NodeType, EdgeType>(nodes_weight_function, nullptr)
                {
                    _output_function = default_function();
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsUnweight<T, U> = 0>
                _RandomFuncGraph(int node_count, int edge_count, int begin_node,
                    bool direction, bool multiply_edge, bool self_loop, bool connect, bool swap_node) :
                    _BasicGraph(node_count, edge_count, begin_node,
                        direction, multiply_edge, self_loop, connect, swap_node, true, true),
                    _RandomFunction<NodeType, EdgeType>(nullptr, nullptr)
                {
                    _output_function = default_function();
                }

                virtual ~_RandomFuncGraph() {}

                std::vector<_Edge<EdgeType>> edges() const { return __get_output_edges(); }
                std::vector<_Edge<EdgeType>>& edges_ref() { return _edges; }

                template<typename T = NodeType, _HasT<T> = 0>
                std::vector<_Node<NodeType>> nodes_weight() const { return _nodes_weight; }
                template<typename T = NodeType, _HasT<T> = 0>
                std::vector<_Node<NodeType>>& nodes_weight_ref() { return _nodes_weight; }
                
                void default_output(std::ostream& os) const {
                    std::vector<int> first_line_vec;
                    __format_output_node(first_line_vec);
                    if (_output_edge_count) {
                        first_line_vec.push_back(_edge_count);
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

                virtual void __format_output_node(std::vector<int>& first_line) const {
                    if (_output_node_count) first_line.push_back(_node_count);
                }
            };

            template<typename NodeType, typename EdgeType>
            class _GenGraph : public _RandomFuncGraph<NodeType, EdgeType>, public _GraphGenSwitch {
            protected:
                using _Self = _GenGraph<NodeType,EdgeType>;
                _OUTPUT_FUNCTION(_Self)
                _DEF_GEN_FUNCTION
            public:
                using _GraphGenSwitch::set_graph_generator;
            public:
                template<typename T = NodeType, typename U = EdgeType, _IsBothWeight<T, U> = 0>
                _GenGraph(int node_count, int edge_count, int begin_node,
                    bool direction, bool multiply_edge, bool self_loop, bool connect, bool swap_node,
                    NodeGenFunction nodes_weight_function,
                    EdgeGenFunction edges_weight_function) :
                    _RandomFuncGraph<NodeType, EdgeType>(node_count, edge_count, begin_node,
                        direction, multiply_edge, self_loop, connect, swap_node,
                        nodes_weight_function, edges_weight_function),
                    _GraphGenSwitch() {}
                
                template<typename T = NodeType, typename U = EdgeType, _IsEdgeWeight<T, U> = 0>
                _GenGraph(int node_count, int edge_count, int begin_node,
                    bool direction, bool multiply_edge, bool self_loop, bool connect, bool swap_node,
                    EdgeGenFunction edges_weight_function) :
                    _RandomFuncGraph<NodeType, EdgeType>(node_count, edge_count, begin_node,
                        direction, multiply_edge, self_loop, connect, swap_node,
                        edges_weight_function),
                    _GraphGenSwitch() {}
                
                template<typename T = NodeType, typename U = EdgeType, _IsNodeWeight<T, U> = 0>
                _GenGraph(int node_count, int edge_count, int begin_node,
                    bool direction, bool multiply_edge, bool self_loop, bool connect, bool swap_node,
                    NodeGenFunction nodes_weight_function) :
                    _RandomFuncGraph<NodeType, EdgeType>(node_count, edge_count, begin_node,
                        direction, multiply_edge, self_loop, connect, swap_node,
                        nodes_weight_function),
                    _GraphGenSwitch() {}
                
                template<typename T = NodeType, typename U = EdgeType, _IsUnweight<T, U> = 0>
                _GenGraph(int node_count, int edge_count, int begin_node,
                    bool direction, bool multiply_edge, bool self_loop, bool connect, bool swap_node) :
                    _RandomFuncGraph<NodeType, EdgeType>(node_count, edge_count, begin_node,
                        direction, multiply_edge, self_loop, connect, swap_node),
                    _GraphGenSwitch() {}

                void gen() { this->_generator->generate(); }
            };

        } // namespace basic
    } // namespace rand_graph
} // namespace generator

#endif // !_SGPCET_GEN_GRAPH_H_
