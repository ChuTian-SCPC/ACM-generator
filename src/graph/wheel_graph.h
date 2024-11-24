#ifndef _SGPCET_WHEEL_GRAPH_H_
#define _SGPCET_WHEEL_GRAPH_H_

#ifndef _SGPCET_GRAPH_H_
#include "graph.h"
#endif // !_SGPCET_GRAPH_H_

namespace generator {
    namespace rand_graph {
        namespace basic {
            template<typename NodeType, typename EdgeType>
            class WheelGraph;

            template <typename NodeType, typename EdgeType>
            class WheelGraphGen : public BasicGraphGen<WheelGraph, NodeType, EdgeType> {
            protected:
                using Context = WheelGraph<NodeType, EdgeType>;
            public:
                WheelGraphGen(Context& graph) : BasicGraphGen<WheelGraph, NodeType, EdgeType>(graph) {}
            
            protected:
                virtual void __self_init() override {
                    this->_context.__init_edge_count();
                }

                virtual void __judge_lower_limit() override {
                    _CONTEXT_GET(node_count)
                    if (node_count < 4) {
                        _msg::__fail_msg(_msg::_defl, 
                            "node_count must greater than or equal to 4, ",
                            tools::string_format("but found %d.", node_count));
                    }
                }

                virtual void __generate_graph() override {
                    _CONTEXT_GET(node_count)
                    std::vector<int> p = rnd.perm(node_count);
                    for (int i = 0; i < node_count; i++) {
                        this->__add_edge(p[i], p[(i + 1) % (node_count - 1)]);
                        this->__add_edge(p[i], p[node_count - 1]);
                    }
                }
            };

            template <typename NodeType, typename EdgeType>
            class WheelGraph : public _GenGraph<NodeType, EdgeType> {
            protected:
                using _Self =  WheelGraph<NodeType,EdgeType>;
                _OUTPUT_FUNCTION(_Self)
                _DEF_GEN_FUNCTION
            public:
                template<typename T = NodeType, typename U = EdgeType, _IsBothWeight<T, U> = 0>
                WheelGraph(int node_count = 4, int begin_node = 1,
                    NodeGenFunction nodes_weight_function = nullptr,
                    EdgeGenFunction edges_weight_function = nullptr) :
                    _GenGraph<NodeType, EdgeType>(node_count, 2 * node_count - 2, begin_node,
                        true, false, false, false, false,
                        nodes_weight_function, edges_weight_function)
                {
                    _TREE_GRAPH_DEFAULT       
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsEdgeWeight<T, U> = 0>
                WheelGraph(int node_count = 4, int begin_node = 1,
                    EdgeGenFunction edges_weight_function = nullptr) :
                    _GenGraph<NodeType, EdgeType>(node_count, 2 * node_count - 2, begin_node,
                        true, false, false, false, false, 
                        edges_weight_function)
                {
                    _TREE_GRAPH_DEFAULT        
                }

                template<typename T = NodeType, typename U = EdgeType, _IsNodeWeight<T, U> = 0>
                WheelGraph(int node_count = 4, int begin_node = 1,
                    NodeGenFunction nodes_weight_function = nullptr) :
                    _GenGraph<NodeType, EdgeType>(node_count, 2 * node_count - 2, begin_node,
                        true, false, false, false, false,
                        nodes_weight_function)
                {
                    _TREE_GRAPH_DEFAULT        
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsUnweight<T, U> = 0>
                WheelGraph(int node_count = 4, int begin_node = 1) :
                    _GenGraph<NodeType, EdgeType>(node_count, 2 * node_count - 2, begin_node,
                        true, false, false, false, false)
                {
                    _TREE_GRAPH_DEFAULT       
                } 
                
                _DISABLE_CONNECT
                _DISABLE_MULTIPLY_EDGE
                _DISABLE_SELF_LOOP
                _DISABLE_EDGE_COUNT
                _OUTPUT_FUNCTION_SETTING(_Self)

                void __init_edge_count() {
                    this->_edge_count = 2 * this->_node_count - 2;
                }
            protected:
                _DEFAULT_GEN_FUNC(WheelGraph)
            };   
        } // namespace basic
    } // namespace rand_graph
} // namespace generator

#endif // !_SGPCET_WHEEL_GRAPH_H_
