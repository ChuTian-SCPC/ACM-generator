#ifndef _SGPCET_DAG_H_
#define _SGPCET_DAG_H_

#ifndef _SGPCET_GRAPH_H_
#include "graph.h"
#endif // !_SGPCET_GRAPH_H_

namespace generator {
    namespace rand_graph {
        namespace basic {
            template<typename NodeType, typename EdgeType>
            class DAG;

            template <typename NodeType, typename EdgeType>
            class DAGGen : public BasicGraphGen<DAG, NodeType, EdgeType> {
            protected:
                using Context = DAG<NodeType, EdgeType>;
                std::vector<int> _rank;
            public:
                DAGGen(Context& graph) : BasicGraphGen<DAG, NodeType, EdgeType>(graph) {}
            
            protected:

                virtual void __self_init() override{
                    _rank = rnd.perm(_CONTEXT_V(node_count), 0);
                }

                virtual void __generate_connect() override{
                    for (int i = 1; i < _CONTEXT_V(node_count); i++) {
                        int f = rnd.next(i);
                        this->__add_edge(_rank[f], _rank[i]);
                    }
                }

                virtual _Edge<EdgeType> __rand_edge() override {
                    int u, v;
                    _CONTEXT_GET(node_count)
                    do {
                        u = rnd.next(node_count);
                        v = rnd.next(node_count);
                        if (u > v) std::swap(u, v);
                        u = _rank[u];
                        v = _rank[v];
                    } while (this->__judge_self_loop(u, v) || this->__judge_multiply_edge(u, v));
                    return this->__convert_edge(u, v);
                }
            };

            template <typename NodeType, typename EdgeType>
            class DAG : public _GenGraph<NodeType, EdgeType> {
            protected:
                using _Self =  DAG<NodeType,EdgeType>;
                _OUTPUT_FUNCTION(_Self)
                _DEF_GEN_FUNCTION
            public:
                template<typename T = NodeType, typename U = EdgeType, _IsBothWeight<T, U> = 0>
                DAG(int node_count = 1, int edge_count = 0, int begin_node = 1,
                    NodeGenFunction nodes_weight_function = nullptr,
                    EdgeGenFunction edges_weight_function = nullptr) :
                    _GenGraph<NodeType, EdgeType>(node_count, edge_count, begin_node,
                        true, false, false, false, false,
                        nodes_weight_function, edges_weight_function)
                {
                    _TREE_GRAPH_DEFAULT       
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsEdgeWeight<T, U> = 0>
                DAG(int node_count = 1, int edge_count = 0, int begin_node = 1,
                    EdgeGenFunction edges_weight_function = nullptr) :
                    _GenGraph<NodeType, EdgeType>(node_count, edge_count, begin_node,
                        true, false, false, false, false, 
                        edges_weight_function)
                {
                    _TREE_GRAPH_DEFAULT        
                }

                template<typename T = NodeType, typename U = EdgeType, _IsNodeWeight<T, U> = 0>
                DAG(int node_count = 1, int edge_count = 0, int begin_node = 1,
                    NodeGenFunction nodes_weight_function = nullptr) :
                    _GenGraph<NodeType, EdgeType>(node_count, edge_count, begin_node,
                        true, false, false, false, false,
                        nodes_weight_function)
                {
                    _TREE_GRAPH_DEFAULT        
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsUnweight<T, U> = 0>
                DAG(int node_count = 1, int edge_count = 0, int begin_node = 1) :
                    _GenGraph<NodeType, EdgeType>(node_count, edge_count, begin_node,
                        true, false, false, false, false)
                {
                    _TREE_GRAPH_DEFAULT       
                } 
                
                _DISABLE_SELF_LOOP
                _DISABLE_DIRECTION
                _OUTPUT_FUNCTION_SETTING(_Self)

                virtual long long max_edge_count() override {
                    if (this->_multiply_edge) {
                        return this->_node_count == 1 ? 0 : _setting::_edge_count_inf;
                    } else {
                        long long n = this->_node_count;
                        return n * (n - 1) / 2;
                    }                   
                }
            protected:
                _DEFAULT_GRAPH_GEN_FUNC(DAG)
            };   
        } // namespace basic
    } // namespace rand_graph
} // namespace generator

#endif // !_SGPCET_DAG_H_
