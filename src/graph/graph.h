#ifndef _SGPCET_GRAPH_H_
#define _SGPCET_GRAPH_H_

#ifndef _SGPCET_GEN_GRAPH_H_
#include "gen_graph.h"
#endif // !_SGPCET_GEN_GRAPH_H_

namespace generator {
    namespace rand_graph {
        namespace basic {
            template<typename NodeType, typename EdgeType>
            class Graph;

            template <typename NodeType, typename EdgeType>
            class GraphGen : public BasicGraphGen<Graph, NodeType, EdgeType> {
            protected:
                using Context = Graph<NodeType, EdgeType>;
            public:
                GraphGen(Context& graph) : BasicGraphGen<Graph, NodeType, EdgeType>(graph) {}
            };

            template <typename NodeType, typename EdgeType>
            class Graph : public _GenGraph<NodeType, EdgeType> {
            protected:
                using _Self =  Graph<NodeType,EdgeType>;
                _OUTPUT_FUNCTION(_Self)
                _DEF_GEN_FUNCTION
            public:
                template<typename T = NodeType, typename U = EdgeType, _IsBothWeight<T, U> = 0>
                Graph(int node_count = 1, int edge_count = 0, int begin_node = 1,
                    NodeGenFunction nodes_weight_function = nullptr,
                    EdgeGenFunction edges_weight_function = nullptr) :
                    _GenGraph<NodeType, EdgeType>(node_count, edge_count, begin_node,
                        false, false, false, false, true,
                        nodes_weight_function, edges_weight_function) 
                {
                    _TREE_GRAPH_DEFAULT       
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsEdgeWeight<T, U> = 0>
                Graph(int node_count = 1, int edge_count = 0, int begin_node = 1,
                    EdgeGenFunction edges_weight_function = nullptr) :
                    _GenGraph<NodeType, EdgeType>(node_count, edge_count, begin_node,
                        false, false, false, false, true, 
                        edges_weight_function)
                {
                    _TREE_GRAPH_DEFAULT        
                }

                template<typename T = NodeType, typename U = EdgeType, _IsNodeWeight<T, U> = 0>
                Graph(int node_count = 1, int edge_count = 0, int begin_node = 1,
                    NodeGenFunction nodes_weight_function = nullptr) :
                    _GenGraph<NodeType, EdgeType>(node_count, edge_count, begin_node,
                        false, false, false, false, true,
                        nodes_weight_function) 
                {
                    _TREE_GRAPH_DEFAULT        
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsUnweight<T, U> = 0>
                Graph(int node_count = 1, int edge_count = 0, int begin_node = 1) :
                    _GenGraph<NodeType, EdgeType>(node_count, edge_count, begin_node,
                        false, false, false, false, true)
                {
                    _TREE_GRAPH_DEFAULT       
                }  
                _OUTPUT_FUNCTION_SETTING(_Self)
            protected:
                _DEFAULT_GEN_FUNC(Graph)
            };   
        } // namespace basic
    } // namespace rand_graph
} // namespace generator

#endif // !_SGPCET_GRAPH_H_
