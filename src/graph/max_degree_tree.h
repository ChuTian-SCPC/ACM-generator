#ifndef _SGPCET_MAX_DEGREE_TREE_H_
#define _SGPCET_MAX_DEGREE_TREE_H_

#ifndef _SGPCET_TREE_H_
#include "tree.h"
#endif // !_SGPCET_TREE_H_

namespace generator {
    namespace rand_graph {
        namespace basic {
            template<typename NodeType, typename EdgeType>
            class MaxDegreeTree;

            template <typename NodeType, typename EdgeType>
            class MaxDegreeTreeGen : public BasicPrueferGen<MaxDegreeTree, NodeType, EdgeType> {
            protected:
                using Context = MaxDegreeTree<NodeType, EdgeType>;
            public:
                MaxDegreeTreeGen(Context& tree) : BasicPrueferGen<MaxDegreeTree, NodeType, EdgeType>(tree) {}
            protected:
                virtual void __generate_tree() override {
                    _CONTEXT_GET(node_count)
                    _CONTEXT_GET(max_degree)
                    std::vector<int> times = rand_array::rand_sum(node_count, node_count - 2, 0, max_degree - 1);
                    std::vector<int> pruefer = rand_array::shuffle_index(times);
                    this->__pruefer_decode(pruefer);
                }

                virtual void __self_init() override {
                    _CONTEXT_GET_REF(max_degree)
                    _CONTEXT_GET(node_count)
                    if (max_degree == -1) {
                        if (node_count == 1) max_degree = 0;
                        else if (node_count == 2) max_degree = 1;
                        else max_degree = rnd.next(2, node_count - 1);
                    }
                }

                virtual void __judge_self_limit() override{
                    _CONTEXT_GET(node_count)
                    _CONTEXT_GET(max_degree)
                    if (max_degree > node_count - 1) {
                        _msg::__warn_msg(_msg::_defl,
                            tools::string_format("the max_degree limit %d is greater than node_count - 1(%d)", max_degree, node_count),
                            ", equivalent to use Tree::use_pruefer()");
                    }
                    int max_degree_limit = node_count == 1 ? 0 : (node_count == 2 ? 1 : 2);
                    
                    if (max_degree < max_degree_limit) {
                        _msg::__fail_msg(_msg::_defl,
                            tools::string_format("the max_degree limit of %s node's tree is greater than or equal to %d, but found %d.",
                            node_count > 2 ? "3 or more" : std::to_string(node_count).c_str(),
                            max_degree_limit, max_degree));
                    }
                }

            };

            template <typename NodeType, typename EdgeType>
            class MaxDegreeTree : public _GenTree<NodeType, EdgeType> {
            protected:
                using _Self =  MaxDegreeTree<NodeType,EdgeType>;
                _OUTPUT_FUNCTION(_Self)
                _DEF_GEN_FUNCTION
                int _max_degree;
            public:
                template<typename T = NodeType, typename U = EdgeType, _IsBothWeight<T, U> = 0>
                MaxDegreeTree(int node = 1, int begin_node = 1, bool is_rooted = false, int root = 1, int max_degree = -1,
                    NodeGenFunction nodes_weight_function = nullptr,
                    EdgeGenFunction edges_weight_function = nullptr) :
                    _GenTree<NodeType, EdgeType>(node, begin_node, is_rooted, root,
                        nodes_weight_function, edges_weight_function),
                    _max_degree(max_degree)
                {
                    _TREE_GRAPH_DEFAULT       
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsEdgeWeight<T, U> = 0>
                MaxDegreeTree(int node = 1, int begin_node = 1, bool is_rooted = false, int root = 1, int max_degree = -1,
                    EdgeGenFunction edges_weight_function = nullptr) :
                    _GenTree<NodeType, EdgeType>(node, begin_node, is_rooted, root, edges_weight_function),
                    _max_degree(max_degree)
                {
                    _TREE_GRAPH_DEFAULT        
                }

                template<typename T = NodeType, typename U = EdgeType, _IsNodeWeight<T, U> = 0>
                MaxDegreeTree(int node = 1, int begin_node = 1, bool is_rooted = false, int root = 1, int max_degree = -1,
                    NodeGenFunction nodes_weight_function = nullptr) :
                    _GenTree<NodeType, EdgeType>(node, begin_node, is_rooted, root, nodes_weight_function),
                    _max_degree(max_degree)
                {
                    _TREE_GRAPH_DEFAULT        
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsUnweight<T, U> = 0>
                MaxDegreeTree(int node = 1, int begin_node = 1, bool is_rooted = false, int root = 1, int max_degree = -1) :
                    _GenTree<NodeType, EdgeType>(node, begin_node, is_rooted, root),
                    _max_degree(max_degree)
                {
                    _TREE_GRAPH_DEFAULT       
                }  
                _OUTPUT_FUNCTION_SETTING(_Self)

                _SET_GET_VALUE(int, max_degree)
            protected:
                _DEFAULT_GRAPH_GEN_FUNC(MaxDegreeTree)
            };   
        } // namespace basic
    } // namespace rand_graph
} // namespace generator

#endif // !_SGPCET_MAX_DEGREE_TREE_H_
