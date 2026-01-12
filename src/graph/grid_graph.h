#ifndef _SGPCET_GRID_GRAPH_H_
#define _SGPCET_GRID_GRAPH_H_

#ifndef _SGPCET_GRAPH_H_
#include "graph.h"
#endif // !_SGPCET_GRAPH_H_

namespace generator {
    namespace rand_graph {
        namespace basic {
            template<typename NodeType, typename EdgeType>
            class GridGraph;

            template <typename NodeType, typename EdgeType>
            class GridGraphGen : public BasicGraphGen<GridGraph, NodeType, EdgeType> {
            protected:
                using Context = GridGraph<NodeType, EdgeType>;
                std::vector<int> _rank;
            public:
                GridGraphGen(Context& graph) : BasicGraphGen<GridGraph, NodeType, EdgeType>(graph) {}
            
            protected:

                virtual void __judge_self_limit() override {
                    _CONTEXT_GET(row)
                    _CONTEXT_GET(column)
                    if (row <= 0) {
                        _msg::__fail_msg(_msg::_defl, 
                            tools::string_format("row must greater than 0, but found %d.", row));
                    }
                    if (column <= 0) {
                        _msg::__fail_msg(_msg::_defl, 
                            tools::string_format("column must greater than 0, but found %d.", column));
                    }
                }

                virtual void __self_init() override {
                    _CONTEXT_GET(row)
                    if (row <= 0) this->_context.rand_row();
                    _CONTEXT_GET(node_count)
                    _rank = rnd.perm(node_count, 0);
                }

                virtual void __generate_connect() override {
                    _CONTEXT_GET(row)
                    _CONTEXT_GET(column)
                    _CONTEXT_GET(node_count)
                    for (int i = 0; i < row; i++) {
                        for (int j = 1; j < column; j++) {
                            int x = i * column + j, y = x - 1;
                            if (x >= node_count) continue;
                            this->__add_edge(_rank[x], _rank[y]);
                        }
                        int x = i * column, y = (i + 1) * column;
                        if (x < node_count && y < node_count) {
                            this->__add_edge(_rank[x], _rank[y]);
                        }
                    }
                }

                virtual _Edge<EdgeType> __rand_edge() override {
                    _CONTEXT_GET(node_count)
                    _CONTEXT_GET(row)
                    _CONTEXT_GET(column)
                    int d[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
                    int pos, k, px, py, nxt;
                    do {
                        pos = rnd.next(node_count);
                        k = rnd.next(4);
                        px = pos / column + d[k][0];
                        py = pos % column + d[k][1];
                        nxt = px * column + py;
                    } while (px < 0 || px >= row || py < 0 || py >= column || nxt >= node_count ||
                             this->__judge_multiply_edge(_rank[pos], _rank[nxt]));
                    return this->__convert_edge(_rank[pos], _rank[nxt]);
                }
            };

            template <typename NodeType, typename EdgeType>
            class GridGraph : public _GenGraph<NodeType, EdgeType> {
            protected:
                using _Self =  GridGraph<NodeType,EdgeType>;
                _OUTPUT_FUNCTION(_Self)
                _DEF_GEN_FUNCTION
                int _row, _column;
            public:
                template<typename T = NodeType, typename U = EdgeType, _IsBothWeight<T, U> = 0>
                GridGraph(int node_count = 1, int edge_count = 0, int begin_node = 1, int row = -1,
                    NodeGenFunction nodes_weight_function = nullptr,
                    EdgeGenFunction edges_weight_function = nullptr) :
                    _GenGraph<NodeType, EdgeType>(node_count, edge_count, begin_node,
                        false, false, false, false, true,
                        nodes_weight_function, edges_weight_function),
                    _row(row)
                {
                    _TREE_GRAPH_DEFAULT       
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsEdgeWeight<T, U> = 0>
                GridGraph(int node_count = 1, int edge_count = 0, int begin_node = 1, int row = -1,
                    EdgeGenFunction edges_weight_function = nullptr) :
                    _GenGraph<NodeType, EdgeType>(node_count, edge_count, begin_node,
                        false, false, false, false, true, 
                        edges_weight_function),
                    _row(row)
                {
                    _TREE_GRAPH_DEFAULT        
                }

                template<typename T = NodeType, typename U = EdgeType, _IsNodeWeight<T, U> = 0>
                GridGraph(int node_count = 1, int edge_count = 0, int begin_node = 1, int row = -1,
                    NodeGenFunction nodes_weight_function = nullptr) :
                    _GenGraph<NodeType, EdgeType>(node_count, edge_count, begin_node,
                        false, false, false, false, true,
                        nodes_weight_function),
                    _row(row)
                {
                    _TREE_GRAPH_DEFAULT        
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsUnweight<T, U> = 0>
                GridGraph(int node_count = 1, int edge_count = 0, int begin_node = 1, int row = -1) :
                    _GenGraph<NodeType, EdgeType>(node_count, edge_count, begin_node,
                        false, false, false, false, true),
                    _row(row)
                {
                    _TREE_GRAPH_DEFAULT       
                } 
                
                void set_row(int row) {
                    _row = row; 
                    if (_row != 0) {
                        _column = (this->_node_count + _row - 1) / _row;
                    } 
                }

                void set_column(int column) {
                    _column = column;
                    if (_column != 0) {
                        _row = (this->_node_count + _column - 1) / _column;
                    }
                }

                void set_row_column(int row, int column, int ignore = 0) {
                    long long node = (long long)row * (long long)column - (long long)ignore;
                    if (ignore >= column) {
                        _msg::__set_fail_msg(_msg::_defl, 
                            tools::string_format("the ignored nodes should in range [0, %d), but found %d.",
                            column, ignore));
                        return;
                    }
                    if (node > _setting::node_limit) {
                        _msg::__set_fail_msg(_msg::_defl,
                            tools::string_format("node_count %d * %d - %d = %lld is greater than the node_limit(%d).",
                            row, column, ignore, node, _setting::node_limit));
                        return;
                    }
                    _row = row;
                    _column = column;
                    if (this->_node_count != node) this->set_node_count(node);                  
                }
                _GET_VALUE(int, row)
                _GET_VALUE(int, column)

                _DISABLE_SELF_LOOP
                _OUTPUT_FUNCTION_SETTING(_Self)

                virtual long long max_edge_count() override {
                    if (this->_node_count == 0) return 0;
                    if (this->_multiply_edge) return this->_node_count == 1 ? 0 : _setting::edge_count_inf;
                    if (_row <= 0 || _column <= 0) {
                        long long max_edge = 0;
                        for (int i = 1; i < this->_node_count; i++) {
                            max_edge = std::max(max_edge, __count_edge_count(i, __count_column(i)));
                        }
                        return max_edge;
                    } else {
                        return __count_edge_count(_row, _column);
                    }
                }

                virtual void rand_edge_count(long long from = _setting::auto_edge_limit, long long to = _setting::auto_edge_limit) override {
                    _GenGraph<NodeType, EdgeType>::rand_edge_count(from, to); 
                    if (_row <= 0) rand_row();
                }

                void rand_row() {
                    int n = this->_node_count;
                    if (this->_multiply_edge) _row = rand_numeric::rand_int(1, n);
                    else {
                        std::vector<long long> counts;
                        long long max_edge = __max_edge_count(counts);
                        max_edge = std::min(max_edge, (long long)_setting::edge_limit);
                        if (this->_edge_count > max_edge) {
                            _msg::__warn_msg(_msg::_defl,
                                tools::string_format("edge_count is large than the maximum possible, use upper edges limit %lld.",
                                max_edge));
                            this->_edge_count = max_edge;
                        }
                        std::vector<int> possible;
                        for (int i = 1; i < n; i++) {
                            if (counts[i] >= this->_edge_count) possible.push_back(i);
                        }
                        _row = rnd.any(possible);
                    }
                    _column = __count_column(_row);
                }

            protected:
                _DEFAULT_GRAPH_GEN_FUNC(GridGraph)

                long long __max_edge_count(std::vector<long long>& counts) {
                    long long max_edge = 0;
                    counts.resize(this->_node_count);
                    for (int i = 1; i < this->_node_count; i++) {
                        counts[i] = __count_edge_count(i, __count_column(i));
                        max_edge = std::max(max_edge, counts[i]);
                    }
                    return max_edge;
                }

                int __count_column(int row) {
                    if (row == 0) _msg::__error_msg(_msg::_defl, "row can't be 0.");
                    return (this->_node_count + row - 1) / row;
                }

                long long __count_edge_count(int row, int column) {
                    long long xl = row;
                    long long yl = column;
                    long long sum = xl * (yl - 1) + yl * (xl - 1) - 2 * (xl * yl - this->_node_count);
                    if (this->_direction) sum *= 2;
                    return sum;
                }
            };   
        } // namespace basic
    } // namespace rand_graph
} // namespace generator

#endif // !_SGPCET_GRID_GRAPH_H_
