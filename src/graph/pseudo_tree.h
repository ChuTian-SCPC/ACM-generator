#ifndef _SGPCET_PSEUDO_TREE_H_
#define _SGPCET_PSEUDO_TREE_H_

#ifndef _SGPCET_CYCLE_GRAPH_H_
#include "cycle_graph.h"
#endif // !_SGPCET_CYCLE_GRAPH_H_

namespace generator {
    namespace rand_graph {
        namespace basic {
            template<typename NodeType, typename EdgeType>
            class PseudoTree;

            template <template <typename, typename> class Type, typename NodeType, typename EdgeType>
            class PseudoTreeBasicGen : public BasicGraphGen<Type, NodeType, EdgeType> {
            protected:
                using Context = Type<NodeType, EdgeType>;
                std::vector<int> _rank;
            public:
                PseudoTreeBasicGen(Context& graph) : BasicGraphGen<Type, NodeType, EdgeType>(graph) {}
            
            protected:
                virtual void __self_init() override {
                    this->_context.__init_edge_count();
                    _CONTEXT_GET(node_count)
                    _CONTEXT_GET_REF(cycle)
                    _rank = rnd.perm(node_count, 0);
                    if (cycle == -1) {
                        cycle = rnd.next(3, node_count);
                    }
                }

                virtual void __judge_self_limit() override {
                    _CONTEXT_GET(cycle)
                    _CONTEXT_GET(node_count)
                    if (cycle < 3 || cycle > node_count) {
                        _msg::__fail_msg(_msg::_defl,
                        tools::string_format("cycle size must in range [3, %d], but found %d.",
                        node_count, cycle));
                    }
                }

                virtual void __judge_lower_limit() override {
                    _CONTEXT_GET(node_count)
                    if (node_count < 3) {
                        _msg::__fail_msg(_msg::_defl,
                        tools::string_format("node_count must greater than or equal to 3, but found %d.",
                        node_count));
                    }
                }

                template<typename T = EdgeType, _HasT<T> = 0>
                CycleGraph<void, EdgeType> __get_cycle_graph() {
                    CycleGraph<void, EdgeType> cycle(_CONTEXT_V(cycle), 0, _CONTEXT_V(edges_weight_function));
                    return cycle;
                }

                template<typename T = EdgeType, _NotHasT<T> = 0>
                CycleGraph<void, void> __get_cycle_graph() {
                    CycleGraph<void, void> cycle(_CONTEXT_V(cycle), 0);
                    return cycle;
                }

                void __generate_cycle() {
                    CycleGraph<void, EdgeType> cycle = __get_cycle_graph();
                    cycle.set_swap_node(_CONTEXT_V(swap_node));
                    cycle.gen(); 
                    std::vector <_Edge<EdgeType>> edge = cycle.edges();
                    for (_Edge<EdgeType>& e: edge) {
                        int& u = e.u_ref();
                        int& v = e.v_ref();
                        u = _rank[u];
                        v = _rank[v];
                        this->__add_edge(e);
                    }
                }

                virtual void __generate_other_edges() {
                    for (int i = _CONTEXT_V(cycle); i < _CONTEXT_V(node_count); i++) {
                        int f = rnd.next(i);
                        this->__add_edge(_rank[i], _rank[f]);
                    }
                }

                virtual void __generate_graph() override {
                    __generate_cycle();
                    __generate_other_edges();
                }       
            };

            template <typename NodeType, typename EdgeType>
            class PseudoTreeGen : public PseudoTreeBasicGen<PseudoTree, NodeType, EdgeType> {
            protected:
                using Context = PseudoTree<NodeType, EdgeType>;
            public:
                PseudoTreeGen(Context& graph) : PseudoTreeBasicGen<PseudoTree, NodeType, EdgeType>(graph) {}
            };

            template <typename NodeType, typename EdgeType>
            class PseudoTree : public _GenGraph<NodeType, EdgeType> {
            protected:
                using _Self =  PseudoTree<NodeType,EdgeType>;
                _OUTPUT_FUNCTION(_Self)
                _DEF_GEN_FUNCTION
                int _cycle;
            public:
                template<typename T = NodeType, typename U = EdgeType, _IsBothWeight<T, U> = 0>
                PseudoTree(int node_count = 3, int begin_node = 1, int cycle = -1,
                    NodeGenFunction nodes_weight_function = nullptr,
                    EdgeGenFunction edges_weight_function = nullptr) :
                    _GenGraph<NodeType, EdgeType>(node_count, node_count, begin_node,
                        false, false, false, true, true,
                        nodes_weight_function, edges_weight_function),
                    _cycle(cycle)
                {
                    _TREE_GRAPH_DEFAULT       
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsEdgeWeight<T, U> = 0>
                PseudoTree(int node_count = 3, int begin_node = 1, int cycle = -1,
                    EdgeGenFunction edges_weight_function = nullptr) :
                    _GenGraph<NodeType, EdgeType>(node_count, node_count, begin_node,
                        false, false, false, true, true, 
                        edges_weight_function),
                    _cycle(cycle)
                {
                    _TREE_GRAPH_DEFAULT        
                }

                template<typename T = NodeType, typename U = EdgeType, _IsNodeWeight<T, U> = 0>
                PseudoTree(int node_count = 3, int begin_node = 1, int cycle = -1,
                    NodeGenFunction nodes_weight_function = nullptr) :
                    _GenGraph<NodeType, EdgeType>(node_count, node_count, begin_node,
                        false, false, false, true, true,
                        nodes_weight_function),
                    _cycle(cycle)
                {
                    _TREE_GRAPH_DEFAULT        
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsUnweight<T, U> = 0>
                PseudoTree(int node_count = 3, int begin_node = 1, int cycle = -1) :
                    _GenGraph<NodeType, EdgeType>(node_count, node_count, begin_node,
                        false, false, false, false, true),
                    _cycle(cycle)
                {
                    _TREE_GRAPH_DEFAULT       
                } 

                void __init_edge_count() {
                    this->_edge_count = this->_node_count;
                }

                _SET_GET_VALUE(int, cycle)

                _DISABLE_EDGE_COUNT
                _DISABLE_CONNECT
                _DISABLE_DIRECTION
                _DISABLE_SELF_LOOP
                _DISABLE_MULTIPLY_EDGE
                _OUTPUT_FUNCTION_SETTING(_Self)
            protected:
                _DEFAULT_GEN_FUNC(PseudoTree)
            }; 

            template <typename NodeType, typename EdgeType>
            class PseudoInTree;

            template <typename NodeType, typename EdgeType>
            class PseudoInTreeGen : public PseudoTreeBasicGen<PseudoInTree, NodeType, EdgeType> {
            protected:
                using Context = PseudoInTree<NodeType, EdgeType>;
            public:
                PseudoInTreeGen(Context& graph) : PseudoTreeBasicGen<PseudoInTree, NodeType, EdgeType>(graph) {}
            };

            template <typename NodeType, typename EdgeType>
            class PseudoInTree : public PseudoTree<NodeType, EdgeType> {
            protected:
                using _Self =  PseudoInTree<NodeType,EdgeType>;
                _OUTPUT_FUNCTION(_Self)
                _DEF_GEN_FUNCTION
            public:
                template<typename T = NodeType, typename U = EdgeType, _IsBothWeight<T, U> = 0>
                PseudoInTree(int node_count = 3, int begin_node = 1, int cycle = -1,
                    NodeGenFunction nodes_weight_function = nullptr,
                    EdgeGenFunction edges_weight_function = nullptr) :
                    PseudoTree<NodeType, EdgeType>(node_count, begin_node, cycle,
                        nodes_weight_function, edges_weight_function)
                {
                    _TREE_GRAPH_DEFAULT
                    this->_direction = true;
                    this->_swap_node = false;       
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsEdgeWeight<T, U> = 0>
                PseudoInTree(int node_count = 3, int begin_node = 1, int cycle = -1,
                    EdgeGenFunction edges_weight_function = nullptr) :
                    PseudoTree<NodeType, EdgeType>(node_count, begin_node, cycle,
                        edges_weight_function)
                {
                    _TREE_GRAPH_DEFAULT
                    this->_direction = true;
                    this->_swap_node = false;    
                }

                template<typename T = NodeType, typename U = EdgeType, _IsNodeWeight<T, U> = 0>
                PseudoInTree(int node_count = 3, int begin_node = 1, int cycle = -1,
                    NodeGenFunction nodes_weight_function = nullptr) :
                    PseudoTree<NodeType, EdgeType>(node_count, begin_node, cycle,
                        nodes_weight_function)
                {
                    _TREE_GRAPH_DEFAULT
                    this->_direction = true;
                    this->_swap_node = false;         
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsUnweight<T, U> = 0>
                PseudoInTree(int node_count = 3, int begin_node = 1, int cycle = -1) :
                    PseudoTree<NodeType, EdgeType>(node_count, begin_node, cycle)
                {
                    _TREE_GRAPH_DEFAULT
                    this->_direction = true;
                    this->_swap_node = false;        
                }   
                _OUTPUT_FUNCTION_SETTING(_Self)
            protected:
                _DEFAULT_GEN_FUNC(PseudoInTree) 
            };

            template <typename NodeType, typename EdgeType>
            class PseudoOutTree;

            template <typename NodeType, typename EdgeType>
            class PseudoOutTreeGen : public PseudoTreeBasicGen<PseudoOutTree, NodeType, EdgeType> {
            protected:
                using Context = PseudoOutTree<NodeType, EdgeType>;
            public:
                PseudoOutTreeGen(Context& graph) : PseudoTreeBasicGen<PseudoOutTree, NodeType, EdgeType>(graph) {}
            protected:

                virtual void __generate_other_edges() override {
                    for (int i = _CONTEXT_V(cycle); i < _CONTEXT_V(node_count); i++) {
                        int f = rnd.next(i);
                        this->__add_edge(this->_rank[f], this->_rank[i]);
                    }
                }
            };

            template <typename NodeType, typename EdgeType>
            class PseudoOutTree : public PseudoTree<NodeType, EdgeType> {
            protected:
                using _Self =  PseudoOutTree<NodeType,EdgeType>;
                _OUTPUT_FUNCTION(_Self)
                _DEF_GEN_FUNCTION
            public:
                template<typename T = NodeType, typename U = EdgeType, _IsBothWeight<T, U> = 0>
                PseudoOutTree(int node_count = 3, int begin_node = 1, int cycle = -1,
                    NodeGenFunction nodes_weight_function = nullptr,
                    EdgeGenFunction edges_weight_function = nullptr) :
                    PseudoTree<NodeType, EdgeType>(node_count, begin_node, cycle,
                        nodes_weight_function, edges_weight_function)
                {
                    _TREE_GRAPH_DEFAULT
                    this->_direction = true;
                    this->_swap_node = false;       
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsEdgeWeight<T, U> = 0>
                PseudoOutTree(int node_count = 3, int begin_node = 1, int cycle = -1,
                    EdgeGenFunction edges_weight_function = nullptr) :
                    PseudoTree<NodeType, EdgeType>(node_count, begin_node, cycle,
                        edges_weight_function)
                {
                    _TREE_GRAPH_DEFAULT
                    this->_direction = true;
                    this->_swap_node = false;    
                }

                template<typename T = NodeType, typename U = EdgeType, _IsNodeWeight<T, U> = 0>
                PseudoOutTree(int node_count = 3, int begin_node = 1, int cycle = -1,
                    NodeGenFunction nodes_weight_function = nullptr) :
                    PseudoTree<NodeType, EdgeType>(node_count, begin_node, cycle,
                        nodes_weight_function)
                {
                    _TREE_GRAPH_DEFAULT
                    this->_direction = true;
                    this->_swap_node = false;         
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsUnweight<T, U> = 0>
                PseudoOutTree(int node_count = 3, int begin_node = 1, int cycle = -1) :
                    PseudoTree<NodeType, EdgeType>(node_count, begin_node, cycle)
                {
                    _TREE_GRAPH_DEFAULT
                    this->_direction = true;
                    this->_swap_node = false;        
                }   
                _OUTPUT_FUNCTION_SETTING(_Self)
            protected:
                _DEFAULT_GEN_FUNC(PseudoOutTree) 
            };
        } // namespace basic
    } // namespace rand_graph
} // namespace generator

#endif // !_SGPCET_PSEUDO_TREE_H_
