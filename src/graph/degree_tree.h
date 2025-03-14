#ifndef _SGPCET_DEGREE_TREE_H_
#define _SGPCET_DEGREE_TREE_H_

#ifndef _SGPCET_TREE_H_
#include "tree.h"
#endif // !_SGPCET_TREE_H_

namespace generator {
    namespace rand_graph {
        namespace basic {
            template <typename NodeType, typename EdgeType>
            class DegreeTree;

            template <typename NodeType, typename EdgeType>
            class DegreeTreeGen : public BasicPrueferGen<DegreeTree, NodeType, EdgeType> {
            protected:
                using Context = DegreeTree<NodeType, EdgeType>; 
            public:
                DegreeTreeGen(Context& tree) : BasicPrueferGen<DegreeTree, NodeType, EdgeType>(tree) {}
            protected:
                virtual void __generate_pruefer() override {
                    std::vector<int> times;
                    _CONTEXT_GET(degrees)
                    for (auto& d : degrees) times.emplace_back(d - 1);
                    std::vector<int> pruefer = rand_array::shuffle_index(times);
                    this->__pruefer_decode(pruefer);
                }

                virtual void __judge_self_limit() override {
                    int sum = 0;
                    _CONTEXT_GET(node_count)
                    _CONTEXT_GET(degrees)
                    for (auto&d : degrees) sum += d;
                    if (sum != 2 * node_count - 2) {
                        _msg::__fail_msg(_msg::_defl,
                            tools::string_format("the sum of degrees should be %d, but found %d.", 2 * node_count - 2, sum));
                    }
                }

                virtual void __self_init() override {
                   _CONTEXT_GET(node_count)
                   _CONTEXT_GET_REF(degrees)
                    degrees.resize(node_count, -1);
                    if (node_count == 1) {
                        if (degrees[0] < 0) degrees[0] = 0;
                        if (degrees[0] != 0) 
                            _msg::__fail_msg(_msg::_defl, tools::string_format("the degree of 1 node tree should be 0, but found %d.", degrees[0]));
                        return;
                    }
                    int sum = 0;
                    int cnt = 0;
                    for (auto& d : degrees) {
                        if (d < 0) cnt++;
                        else if (d == 0) _msg::__fail_msg(_msg::_defl, "the degree of node can't be 0.");
                        else sum += d;
                    }
                    if (sum > 2 * node_count - 2) {
                        _msg::__fail_msg(_msg::_defl, 
                            tools::string_format("the sum of degree can't be greater than %d, but found.", 2 * node_count - 2, sum));
                    }
                    std::vector<int> rand_degrees = rand_array::rand_sum(cnt, 2 * node_count - 2 - sum, 1);
                    int j = 0;
                    for (auto& d : degrees) {
                        if (d < 0) {
                            d = rand_degrees[j];
                            j++;
                        }
                    }
                }
            };

            template <typename NodeType, typename EdgeType>
            class DegreeTree : public _GenTree<NodeType, EdgeType> {
            protected:
                using _Self = DegreeTree<NodeType, EdgeType>;
                _OUTPUT_FUNCTION(_Self)
                _DEF_GEN_FUNCTION
                std::vector<int> _degrees;
            public:
                template<typename T = NodeType, typename U = EdgeType, _IsBothWeight<T, U> = 0>
                DegreeTree(int node_count = 1, int begin_node = 1, bool is_rooted = false, int root = 1,
                    NodeGenFunction nodes_weight_function = nullptr,
                    EdgeGenFunction edges_weight_function = nullptr) :
                    _GenTree<NodeType, EdgeType>(node_count, begin_node, is_rooted, root,
                        nodes_weight_function, edges_weight_function)
                {
                    _TREE_GRAPH_DEFAULT
                }

                template<typename T = NodeType, typename U = EdgeType, _IsEdgeWeight<T, U> = 0>
                DegreeTree(int node_count = 1, int begin_node = 1, bool is_rooted = false, int root = 1,
                    EdgeGenFunction edges_weight_function = nullptr):
                    _GenTree<NodeType, EdgeType>(node_count, begin_node, is_rooted, root, edges_weight_function)
                {
                    _TREE_GRAPH_DEFAULT
                }

                template<typename T = NodeType, typename U = EdgeType, _IsNodeWeight<T, U> = 0>
                DegreeTree(int node_count = 1, int begin_node = 1, bool is_rooted = false, int root = 1,
                    NodeGenFunction nodes_weight_function = nullptr) :
                    _GenTree<NodeType, EdgeType>(node_count, begin_node, is_rooted, root, nodes_weight_function)
                {
                    _TREE_GRAPH_DEFAULT
                }

                template<typename T = NodeType, typename U = EdgeType, _IsUnweight<T, U> = 0>
                DegreeTree(int node_count = 1, int begin_node = 1, bool is_rooted = false, int root = 1) :
                    _GenTree<NodeType, EdgeType>(node_count, begin_node, is_rooted, root)
                {
                    _TREE_GRAPH_DEFAULT
                }

                _OUTPUT_FUNCTION_SETTING(_Self)

                _SET_GET_VALUE(std::vector<int>, degrees)

                void set_degrees(int index, int degree) {
                    if (index <= 0 && index > this->_node_count) {
                        _msg::__set_fail_msg(_msg::_defl, 
                            tools::string_format("the index should in range [1, %d], but found %d.", this->_node_count, index));
                        return;
                    }
                    if (_degrees.size() < index) _degrees.resize(this->_node_count, -1);
                    _degrees[index - 1] = degree;
                }

            protected:


                _DEFAULT_GRAPH_GEN_FUNC(DegreeTree)
            };
        } // namespace basic
    } // namespace rand_graph
} // namespace generator

#endif // _SGPCET_DEGREE_TREE_H_
