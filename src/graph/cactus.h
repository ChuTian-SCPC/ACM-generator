#ifndef _SGPCET_CACTUS_H_
#define _SGPCET_CACTUS_H_

#ifndef _SGPCET_CYCLE_GRAPH_H_
#include "cycle_graph.h"
#endif // !_SGPCET_CYCLE_GRAPH_H_

namespace generator {
    namespace rand_graph {
        namespace basic {
            template<typename NodeType, typename EdgeType>
            class Cactus;

            template <typename NodeType, typename EdgeType>
            class CactusGen : public BasicGraphGen<Cactus, NodeType, EdgeType> {
            protected:
                using Context = Cactus<NodeType, EdgeType>;
                std::vector<int> _rank;
            public:
                CactusGen(Context& graph) : BasicGraphGen<Cactus, NodeType, EdgeType>(graph) {}
            protected:
                virtual void __self_init() override{
                    _rank = rnd.perm(_CONTEXT_V(node_count));
                }

                template<typename T = EdgeType, _HasT<T> = 0>
                CycleGraph<void, EdgeType> __get_cycle_graph(int size) {
                    CycleGraph<void, EdgeType> cycle(size, 0, _CONTEXT_V(edges_weight_function));
                    return cycle;
                }

                template<typename T = EdgeType, _NotHasT<T> = 0>
                CycleGraph<void, void> __get_cycle_graph(int size) {
                    CycleGraph<void, void> cycle(size, 0);
                    return cycle;
                }

                virtual void __generate_graph() override {
                    std::vector<std::vector<int>> cycles;
                    _CONTEXT_GET(node_count)
                    _CONTEXT_GET(edge_count)
                    int m = edge_count - (node_count - 1);
                    for (int i = 2; i <= 2 * m; i += 2) {
                        std::vector<int> pre;
                        if (i == 2) {
                            pre.emplace_back(0);
                        }
                        pre.emplace_back(i);
                        pre.emplace_back(i - 1);
                        cycles.emplace_back(pre);
                    }
                    int len = cycles.size();
                    int add = len == 0 ? 0 : rnd.next(0, node_count - (2 * m + 1));
                    for (int i = 2 * m + 1; i <= 2 * m + add; i++) {
                        int w = rnd.next(len);
                        cycles[w].emplace_back(i);
                    }
                    for (int i = 2 * m + add + (len != 0); i < node_count; i++) {
                        cycles.emplace_back(1, i);
                    }
                    shuffle(cycles.begin() + 1, cycles.end());
                    for(size_t i = 0; i < cycles.size(); i++) {
                        std::vector<int> current = cycles[i];
                        if (i != 0) {
                            int w = rnd.next(i);
                            current.push_back(rnd.any(cycles[w]));
                        }
                        if(current.size() == 1) {
                            continue;
                        }
                        else if(current.size() == 2) {
                            this->__add_edge(_rank[current[0]], _rank[current[1]]);
                        }
                        else {
                            CycleGraph<void, EdgeType> cycle = __get_cycle_graph(current.size());
                            cycle.gen();
                            std::vector<_Edge<EdgeType>> edge = cycle.edges();
                            for(_Edge<EdgeType>& e : edge) {
                                int& u = e.u_ref();
                                int& v = e.v_ref();
                                u = _rank[current[u]];
                                v = _rank[current[v]];
                                this->__add_edge(e);
                            }
                        }
                    }
                }
            };

            template <typename NodeType, typename EdgeType>
            class Cactus : public _GenGraph<NodeType, EdgeType> {
            protected:
                using _Self =  Cactus<NodeType,EdgeType>;
                _OUTPUT_FUNCTION(_Self)
                _DEF_GEN_FUNCTION
            public:
                template<typename T = NodeType, typename U = EdgeType, _IsBothWeight<T, U> = 0>
                Cactus(int node_count = 1, int edge_count = 0, int begin_node = 1, 
                    NodeGenFunction nodes_weight_function = nullptr,
                    EdgeGenFunction edges_weight_function = nullptr) :
                    _GenGraph<NodeType, EdgeType>(node_count, edge_count, begin_node,
                         false, false, false, true, true,
                        nodes_weight_function, edges_weight_function) 
                {
                    _TREE_GRAPH_DEFAULT       
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsEdgeWeight<T, U> = 0>
                Cactus(int node_count = 1, int edge_count = 0, int begin_node = 1, 
                    EdgeGenFunction edges_weight_function = nullptr) :
                    _GenGraph<NodeType, EdgeType>(node_count, edge_count, begin_node,
                         false, false, false, true, true,
                         edges_weight_function)
                {
                    _TREE_GRAPH_DEFAULT        
                }

                template<typename T = NodeType, typename U = EdgeType, _IsNodeWeight<T, U> = 0>
                Cactus(int node_count = 1, int edge_count = 0, int begin_node = 1, 
                    NodeGenFunction nodes_weight_function = nullptr) :
                    _GenGraph<NodeType, EdgeType>(node_count, edge_count, begin_node,
                        false, false, false, true, true,
                        nodes_weight_function) 
                {
                    _TREE_GRAPH_DEFAULT        
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsUnweight<T, U> = 0>
                Cactus(int node_count = 1, int edge_count = 0, int begin_node = 1) :
                    _GenGraph<NodeType, EdgeType>(node_count, edge_count, begin_node,
                        false, false, false, true, true)
                {
                    _TREE_GRAPH_DEFAULT       
                }  

                _DISABLE_DIRECTION
                _DISABLE_CONNECT
                _DISABLE_SELF_LOOP
                _DISABLE_MULTIPLY_EDGE
                _OUTPUT_FUNCTION_SETTING(_Self)

                virtual long long max_edge_count() override {
                    long long n = this->_node_count;
                    return n - 1 + (n - 1) / 2;
                }
            protected:
                _DEFAULT_GRAPH_GEN_FUNC(Cactus)
            };   
        } // namespace basic
    } // namespace rand_graph
} // namespace generator

#endif // !_SGPCET_CACTUS_H_
