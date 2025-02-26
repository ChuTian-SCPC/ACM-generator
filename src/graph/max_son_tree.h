#ifndef _SGPCET_MAX_SON_TREE_H_
#define _SGPCET_MAX_SON_TREE_H_

#ifndef _SGPCET_TREE_H_
#include "tree.h"
#endif // !_SGPCET_TREE_H_

namespace generator {
    namespace rand_graph {
        namespace basic {
            template<typename NodeType, typename EdgeType>
            class MaxSonTree;

            template <typename NodeType, typename EdgeType>
            class MaxSonTreeGen : public BasicPrueferGen<MaxSonTree, NodeType, EdgeType> {
            protected:
                using Context = MaxSonTree<NodeType, EdgeType>;
            public:
                MaxSonTreeGen(Context& tree) : BasicPrueferGen<MaxSonTree, NodeType, EdgeType>(tree) {}
            protected:
                virtual void __generate_tree() override {
                    _CONTEXT_GET(max_son)
                    _CONTEXT_GET(node_count)
                    int max_degree = max_son + 1;
                    std::vector<int> times = rand_array::rand_sum(node_count, node_count - 2, 0, max_degree - 1);
                    _CONTEXT_GET_REF(root)
                    if (times[root] == max_degree - 1) {
                        int p;
                        do {
                            p = rnd.next(0, node_count - 1);
                        } while (p == root || times[p] == max_degree - 1);
                        std::swap(times[root], times[p]);
                    }
                    std::vector<int> pruefer = rand_array::shuffle_index(times);
                    this->__pruefer_decode(pruefer);
                }

                virtual void __self_init() override {
                    _CONTEXT_GET_REF(max_son)
                    _CONTEXT_GET(node_count)
                    if (max_son == -1) {
                        if (node_count == 1) max_son = 0;
                        else if (node_count == 2) max_son = 1;
                        else max_son = rnd.next(2, node_count - 1);
                    }
                }

                virtual void __judge_self_limit() override{
                    _CONTEXT_GET(node_count)
                    _CONTEXT_GET(max_son)
                    if (max_son > node_count - 1) {
                        _msg::__warn_msg(_msg::_defl,
                            tools::string_format("the max_son limit %d is greater than node_count - 1(%d)", max_son, node_count),
                            ", equivalent to use Tree::use_pruefer()");
                    }
                    int max_son_limit = node_count == 1 ? 0 : 1;
                    
                    if (max_son < max_son_limit) {
                        _msg::__fail_msg(_msg::_defl,
                            tools::string_format("the max_son limit of %s node's tree is greater than or equal to %d, but found %d.",
                            node_count > 2 ? "3 or more" : std::to_string(node_count).c_str(),
                            max_son_limit, max_son));
                    }
                }

            };

            template <typename NodeType, typename EdgeType>
            class MaxSonTree : public _GenTree<NodeType, EdgeType> {
            protected:
                using _Self =  MaxSonTree<NodeType,EdgeType>;
                _OUTPUT_FUNCTION(_Self)
                _DEF_GEN_FUNCTION
                int _max_son;
            public:
                template<typename T = NodeType, typename U = EdgeType, _IsBothWeight<T, U> = 0>
                MaxSonTree(int node = 1, int begin_node = 1, int root = 1, int max_son = -1,
                    NodeGenFunction nodes_weight_function = nullptr,
                    EdgeGenFunction edges_weight_function = nullptr) :
                    _GenTree<NodeType, EdgeType>(node, begin_node, true, root,
                        nodes_weight_function, edges_weight_function),
                    _max_son(max_son)
                {
                    _TREE_GRAPH_DEFAULT       
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsEdgeWeight<T, U> = 0>
                MaxSonTree(int node = 1, int begin_node = 1, int root = 1, int max_son = -1,
                    EdgeGenFunction edges_weight_function = nullptr) :
                    _GenTree<NodeType, EdgeType>(node, begin_node, true, root, edges_weight_function),
                    _max_son(max_son)
                {
                    _TREE_GRAPH_DEFAULT        
                }

                template<typename T = NodeType, typename U = EdgeType, _IsNodeWeight<T, U> = 0>
                MaxSonTree(int node = 1, int begin_node = 1, int root = 1, int max_son = -1,
                    NodeGenFunction nodes_weight_function = nullptr) :
                    _GenTree<NodeType, EdgeType>(node, begin_node, true, root, nodes_weight_function),
                    _max_son(max_son)
                {
                    _TREE_GRAPH_DEFAULT        
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsUnweight<T, U> = 0>
                MaxSonTree(int node = 1, int begin_node = 1, int root = 1, int max_son = -1) :
                    _GenTree<NodeType, EdgeType>(node, begin_node, true, root),
                    _max_son(max_son)
                {
                    _TREE_GRAPH_DEFAULT       
                }  
                _OUTPUT_FUNCTION_SETTING(_Self)

                _SET_GET_VALUE(int, max_son)
                _MUST_IS_ROOTED
            protected:
                _DEFAULT_GRAPH_GEN_FUNC(MaxSonTree)
            };   
        } // namespace basic
    } // namespace rand_graph
} // namespace generator

#endif // !_SGPCET_MAX_SON_TREE_H_
