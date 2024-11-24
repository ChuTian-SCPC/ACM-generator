#ifndef _SGPCET_BIPARTITE_GRAPH_H_
#define _SGPCET_BIPARTITE_GRAPH_H_

#ifndef _SGPCET_GRAPH_H_
#include "graph.h"
#endif // !_SGPCET_GRAPH_H_

namespace generator {
    namespace rand_graph {
        namespace basic {
            template<typename NodeType, typename EdgeType>
            class BipartiteGraph;

            template <typename NodeType, typename EdgeType>
            class BipartiteGraphGen : public BasicGraphGen<BipartiteGraph, NodeType, EdgeType> {
            protected:
                using Context = BipartiteGraph<NodeType, EdgeType>;
                std::vector<int> _degree[2];
                std::vector<int> _part[2];
                int _d[2];
            public:
                BipartiteGraphGen(Context& graph) : BasicGraphGen<BipartiteGraph, NodeType, EdgeType>(graph) {}
            
            protected:
                void __rand_left() {
                    _CONTEXT_GET_REF(left)
                    _CONTEXT_GET_REF(right)
                    if (left >= 0) return;
                    _CONTEXT_GET(node_count)
                    _CONTEXT_GET(edge_count)
                    int l = 0, r = node_count / 2, limit;
                    if (!_CONTEXT_V(multiply_edge)) {
                        if (edge_count > r * (node_count - r)) {
                            _msg::__fail_msg(_msg::_defl,
                                tools::string_format("edges_count must less than or equal to %d, but found %d.",
                                r * (node_count - r), edge_count));
                        }
                        while (l <= r) {
                            int mid = (l + r) / 2;
                            if (mid * (node_count - mid) < edge_count) {
                                l = mid + 1;
                            } else {
                                limit = r;
                                r = mid - 1;
                            }
                        }
                    } else {
                        limit = 1;
                    }
                    left = rnd.next(limit, node_count / 2);
                    right = node_count - left;
                }   

                void __remark_node_indices_by_part(std::vector<int>& part) {
                    int index = _CONTEXT_V(begin_node);
                    _CONTEXT_GET_REF(node_indices)
                    for(int x : part) {                   
                        node_indices[x] = index;
                        index++;
                    }
                }

                void __remark_node_indices() {
                    if (_CONTEXT_V(different_part)) {
                        if (_part[0].empty() && _part[1].empty()) {
                            return;
                        }
                        _CONTEXT_V_REF(node_indices).resize(_CONTEXT_V(node_count));
                        __remark_node_indices_by_part(_part[0]);
                        __remark_node_indices_by_part(_part[1]);
                    }
                    else {
                        this->_context.default_node_indices();
                    }
                }

                virtual void __self_init() override {
                    __rand_left();
                    _CONTEXT_GET(node_count)
                    _CONTEXT_GET_REF(left)
                    _CONTEXT_GET_REF(right)
                    right = node_count - left;
                    
                    for (int i = 0; i < 2; i++) {
                        _part[i].clear();
                        _degree[i].clear();
                    }
                    std::vector<int> p = rnd.perm(node_count);
                    for (int i = 0; i < left; i++) {
                        _part[0].push_back(p[i]);
                    }
                    for (int i = left; i < node_count; i++) {
                        _part[1].push_back(p[i]);
                    }
                    if (_CONTEXT_V(different_part)) {
                        __remark_node_indices();
                    }
                    if (_CONTEXT_V(connect)) {
                        _degree[0].resize(left, 1);
                        _degree[1].resize(right, 1);
                        for (int i = left; i < node_count - 1; i++) {
                            _degree[0][rnd.next(left)]++;
                        }
                        for (int i = right; i < node_count - 1; i++) {
                            _degree[1][rnd.next(right)]++;
                        }
                        _d[0] = node_count - 1;
                        _d[1] = node_count - 1;
                    }
                }

                virtual void __judge_self_limit() override {
                    _CONTEXT_GET(left)
                    _CONTEXT_GET(right)
                    if (left < 0) {
                        _msg::__fail_msg(_msg::_defl,
                            "left part size must greater than or equal to 0,",
                            tools::string_format("but found %d", left));
                    }
                    if (right < 0) {
                        _msg::__fail_msg(_msg::_defl,
                            "right part size must greater than or equal to 0,",
                            tools::string_format("but found %d", right));
                    }
                }

                virtual void __judge_upper_limit() override {
                    _CONTEXT_GET(node_count)
                    _CONTEXT_GET(edge_count)
                    if (!_CONTEXT_V(multiply_edge)) {
                        long long limit = (long long)_CONTEXT_V(left) * (long long)_CONTEXT_V(right);
                        if (limit < edge_count) {
                            _msg::__fail_msg(_msg::_defl,
                                tools::string_format("number of edges must less than or equal to %lld, but found %d.",
                                limit, edge_count));
                        }
                    }
                    else {
                        if (node_count == 1 && edge_count > 0) {
                            _msg::__fail_msg(_msg::_defl, 
                                tools::string_format("number of edges must equal to 0, but found %d.",
                                edge_count));
                        }
                    }
                }  

                virtual _Edge<EdgeType> __rand_edge() override{
                    int u, v;
                    do {
                        u = rnd.any(_part[0]);
                        v = rnd.any(_part[1]);
                    } while (this->__judge_multiply_edge(u, v));
                    return this->__convert_edge(u, v);
                }

                void __add_part_edge(int f, int i, int j) {
                    int u = _part[f][i];
                    int v = _part[f ^ 1][j];
                    if (f == 1) {
                        std::swap(u, v);
                    }
                    this->__add_edge(u, v);
                    _d[0]--;
                    _d[1]--;
                    _degree[f][i]--;
                    _degree[f ^ 1][j]--;
                } 

                virtual void __generate_connect() override {
                    int f = 0;
                    _CONTEXT_GET(left)
                    _CONTEXT_GET(right)
                    while (_d[0] + _d[1] > 0) {
                        for (int i = 0; i < (f == 0 ? left : right); i++) {
                            if (_degree[f][i] == 1) {
                                if (_d[f] == 1) {
                                    for (int j = 0; j < (f == 0 ? right : left); j++) {
                                        if (_degree[f ^ 1][j] == 1) {
                                            __add_part_edge(f, i, j);
                                        }
                                    }
                                } else {
                                    int j;
                                    do {
                                        j = rnd.next(f == 0 ? right : left);
                                    } while (_degree[f ^ 1][j] < 2);
                                    __add_part_edge(f, i, j);
                                }
                            }
                        }
                        f ^= 1;
                    }
                }  
            };

            template <typename NodeType, typename EdgeType>
            class BipartiteGraph : public _GenGraph<NodeType, EdgeType> {
            public:
                enum NodeOutputFormat {
                    Node,
                    LeftRight,
                    NodeLeft,
                    NodeRight
                };
            protected:
                int _left, _right;
                bool _different_part;
                NodeOutputFormat _node_output_format;
                using _Self =  BipartiteGraph<NodeType,EdgeType>;
                _OUTPUT_FUNCTION(_Self)
                _DEF_GEN_FUNCTION
            public:
                template<typename T = NodeType, typename U = EdgeType, _IsBothWeight<T, U> = 0>
                BipartiteGraph(int node_count = 1, int edge_count = 0, int begin_node = 1, int left = -1,
                    NodeGenFunction nodes_weight_function = nullptr,
                    EdgeGenFunction edges_weight_function = nullptr) :
                    _GenGraph<NodeType, EdgeType>(node_count, edge_count, begin_node,
                        false, false, false, false, false,
                        nodes_weight_function, edges_weight_function),
                    _left(left), _different_part(false), _node_output_format(Node)
                {
                    _TREE_GRAPH_DEFAULT       
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsEdgeWeight<T, U> = 0>
                BipartiteGraph(int node_count = 1, int edge_count = 0, int begin_node = 1, int left = -1,
                    EdgeGenFunction edges_weight_function = nullptr) :
                    _GenGraph<NodeType, EdgeType>(node_count, edge_count, begin_node,
                        false, false, false, false, false, 
                        edges_weight_function),
                    _left(left), _different_part(false), _node_output_format(Node)
                {
                    _TREE_GRAPH_DEFAULT        
                }

                template<typename T = NodeType, typename U = EdgeType, _IsNodeWeight<T, U> = 0>
                BipartiteGraph(int node_count = 1, int edge_count = 0, int begin_node = 1, int left = -1,
                    NodeGenFunction nodes_weight_function = nullptr) :
                    _GenGraph<NodeType, EdgeType>(node_count, edge_count, begin_node,
                        false, false, false, false, false,
                        nodes_weight_function),
                    _left(left), _different_part(false), _node_output_format(Node)
                {
                    _TREE_GRAPH_DEFAULT        
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsUnweight<T, U> = 0>
                BipartiteGraph(int node_count = 1, int edge_count = 0, int begin_node = 1, int left = -1) :
                    _GenGraph<NodeType, EdgeType>(node_count, edge_count, begin_node,
                        false, false, false, false, false),
                    _left(left), _different_part(false), _node_output_format(Node)
                {
                    _TREE_GRAPH_DEFAULT       
                }  

                _SET_GET_VALUE(bool, different_part)

                void set_left(int left) {
                    _left = left;
                    _right = this->_node_count - _left;
                }

                void set_right(int right) {
                    _right = right;
                    _left = this->_node_count - _right;
                }

                void set_left_right(int left, int right) {
                    if (left + right < 0) {
                        _msg::__set_fail_msg(_msg::_defl,
                            "number of left part nodes add right part nodes must greater than 0,",
                            tools::string_format("but found %d + %d = %d",
                            left, right, left + right));
                        return;
                    }
                    _left = left;
                    _right = right;
                    int node_count = left + right;
                    if (this->_node_count != node_count) {
                        _msg::__warn_msg(_msg::_defl,
                            tools::string_format("number of left part nodes add right part nodes is not equal to node_count(%d),", node_count),
                            tools::string_format("set node_count to %d + %d = %d.",
                            left, right, left + right));
                        this->set_node_count(node_count);
                    }                   
                }
                _GET_VALUE(int, left)
                _GET_VALUE(int, right)

                void set_node_output_format(NodeOutputFormat format) { _node_output_format = format; }
                void use_format_node() {  _node_output_format = Node; }
                void use_format_left_right() { _node_output_format = LeftRight; }
                void use_format_node_left() { _node_output_format = NodeLeft; }
                void use_format_node_right() { _node_output_format = NodeRight; }
                _GET_VALUE(NodeOutputFormat, node_output_format)
                
                _DISABLE_SELF_LOOP
                _DISABLE_DIRECTION
                _OUTPUT_FUNCTION_SETTING(_Self)
            protected:
                _DEFAULT_GEN_FUNC(BipartiteGraph)

                virtual void __format_output_node(std::vector<int>& first_line) const override {
                    if (this->_output_node_count) {
                        if (_node_output_format == Node) {
                            first_line.push_back(this->_node_count);
                        } else if (_node_output_format == LeftRight) {
                            first_line.push_back(_left);
                            first_line.push_back(_right);
                        } else if (_node_output_format == NodeLeft) {
                            first_line.push_back(this->_node_count);
                            first_line.push_back(_left);
                        } else if (_node_output_format == NodeRight) {
                            first_line.push_back(this->_node_count);
                            first_line.push_back(_right);
                        }
                    }
                }
            };   
        } // namespace basic
    } // namespace rand_graph
} // namespace generator

#endif // !_SGPCET_BIPARTITE_GRAPH_H_
