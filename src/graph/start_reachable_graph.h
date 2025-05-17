#ifndef _SGPCET_START_REACHABLE_GRAPH_H_
#define _SGPCET_START_REACHABLE_GRAPH_H_

#ifndef _SGPCET_GRAPH_H_
#include "graph.h"
#endif // !_SGPCET_GRAPH_H_

namespace generator {
    namespace rand_graph {
        namespace basic {
            template <typename NodeType, typename EdgeType>
            class StartReachableGraph;

            template <typename NodeType, typename EdgeType>
            class StartReachableGraphGen : public BasicGraphGen<StartReachableGraph, NodeType, EdgeType> {
            protected:
                using Context = StartReachableGraph<NodeType, EdgeType>;
            public: 
                StartReachableGraphGen(Context& context) : BasicGraphGen<StartReachableGraph, NodeType, EdgeType>(context) {}
            protected:
                virtual void __generate_connect() override {
                    _CONTEXT_GET(node_count)
                    _CONTEXT_GET_REF(start)
                    Tree<void, void> tree(node_count, 0, true, start + 1);
                    tree.gen();
                    std::vector <_Edge<void>>& edge = tree.edges_ref();
                    for (auto& e: edge) this->__add_edge(e.u(), e.v());                   
                }
            };

            template <typename NodeType, typename EdgeType>
            class StartReachableGraph : public _GenGraph<NodeType, EdgeType> {
            protected:
                using _Self = StartReachableGraph<NodeType, EdgeType>;
                _OUTPUT_FUNCTION(_Self)
                _DEF_GEN_FUNCTION
                int _start;
            public:
                template<typename T = NodeType, typename U = EdgeType, _IsBothWeight<T, U> = 0>
                StartReachableGraph(int node_count = 1, int edge_count = 0, int begin_node = 1, int start = 1,
                    NodeGenFunction nodes_weight_function = nullptr,
                    EdgeGenFunction edges_weight_function = nullptr) :
                    _GenGraph<NodeType, EdgeType>(node_count, edge_count, begin_node,
                        true, false, false, true, false,
                        nodes_weight_function, edges_weight_function),
                    _start(start - 1)
                {
                    _TREE_GRAPH_DEFAULT
                }

                template<typename T = NodeType, typename U = EdgeType, _IsEdgeWeight<T, U> = 0>
                StartReachableGraph(int node_count = 1, int edge_count = 0, int begin_node = 1, int start = 1,
                    EdgeGenFunction edges_weight_function = nullptr) :
                    _GenGraph<NodeType, EdgeType>(node_count, edge_count, begin_node,
                        true, false, false, true, false,
                        edges_weight_function),
                    _start(start - 1)
                {
                    _TREE_GRAPH_DEFAULT
                }

                template<typename T = NodeType, typename U = EdgeType, _IsNodeWeight<T, U> = 0>
                StartReachableGraph(int node_count = 1, int edge_count = 0, int begin_node = 1, int start = 1,
                    NodeGenFunction nodes_weight_function = nullptr) :
                    _GenGraph<NodeType, EdgeType>(node_count, edge_count, begin_node,
                        true, false, false, true, false,
                        nodes_weight_function),
                    _start(start - 1)
                {
                    _TREE_GRAPH_DEFAULT
                }

                template<typename T = NodeType, typename U = EdgeType, _IsUnweight<T, U> = 0>
                StartReachableGraph(int node_count = 1, int edge_count = 0, int begin_node = 1, int start = 1) :
                    _GenGraph<NodeType, EdgeType>(node_count, edge_count, begin_node,
                        true, false, false, true, false),
                    _start(start - 1)
                {
                    _TREE_GRAPH_DEFAULT 
                }

                _GET_VALUE_REF(int, start)
                void set_start(int start) {
                    if (start < 1 || start > this->_node_count) {
                        _msg::__set_fail_msg(_msg::_defl, tools::string_format("start index should in range[1, %d], but found %d.", 
                            this->_node_count, start));
                        return;
                    }
                    _start = start - 1;
                }

                int start() { return this->_node_indices[_start];}

                _OUTPUT_FUNCTION_SETTING(_Self)
                _DISABLE_CONNECT
                _DISABLE_DIRECTION
            protected:
                _DEFAULT_GRAPH_GEN_FUNC(StartReachableGraph)
            };
        } // namespace basic
    } // namespace rand_graph
} // namespace generator

#endif // ! _SGPCET_START_REACHABLE_GRAPH_H_
