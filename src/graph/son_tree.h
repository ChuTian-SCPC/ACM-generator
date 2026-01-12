#ifndef _SGPCET_SON_TREE_H_
#define _SGPCET_SON_TREE_H_

#ifndef _SGPCET_TREE_H_
#include "tree.h"
#endif // !_SGPCET_TREE_H_

namespace generator {
    namespace rand_graph {
        namespace basic {
            template <typename NodeType, typename EdgeType>
            class SonTree;

            template <typename NodeType, typename EdgeType>
            class SonTreeGen : public BasicPrueferGen<SonTree, NodeType, EdgeType> {
            protected:
                using Context = SonTree<NodeType, EdgeType>;
            public:
                SonTreeGen(Context& tree) : BasicPrueferGen<SonTree, NodeType, EdgeType>(tree) {}
            protected:
                virtual void __generate_pruefer() override {
                    std::vector<int> times;
                    _CONTEXT_GET(sons)
                    _CONTEXT_GET_REF(root)
                    for (int i = 0; i < sons.size(); i++) {
                        if (i == root) times.emplace_back(sons[i] - 1);
                        else times.emplace_back(sons[i]); 
                    }
                    std::vector<int> pruefer = rand_array::shuffle_index(times);
                    this->__pruefer_decode(pruefer);
                }  

                virtual void __judge_self_limit() override {
                    int sum = 0;
                    _CONTEXT_GET(node_count)
                    _CONTEXT_GET(sons)
                    for (auto& s : sons) sum += s; 
                    if (sum != node_count - 1) {
                        _msg::__fail_msg(_msg::_defl,
                            tools::string_format("the sum of sons should be %d, but found %d.", node_count - 1, sum)); 
                    }
                }

                virtual void __self_init() override {
                    _CONTEXT_GET(node_count)
                    _CONTEXT_GET_REF(sons)
                    _CONTEXT_GET_REF(root)
                    sons.resize(node_count, -1);
                    if (node_count == 1) {
                        if (sons[0] < 0) sons[0] = 0;
                        if (sons[0]!= 0)
                            _msg::__fail_msg(_msg::_defl, tools::string_format("the sons of 1 node tree should be 0, but found %d.", sons[0])); 
                        return;
                    } 
                    int sum = 0;
                    int cnt = 0;
                    int p = -1;
                    for (int i = 0; i < sons.size(); i++) {
                        auto s = sons[i];
                        if (s < 0) {
                            if (i == root) p = cnt;
                            cnt++;
                        }
                        else if (i == root && s == 0) 
                            _msg::__fail_msg(_msg::_defl, "the sons of root node can't be 0.");
                        else sum += s;
                    }
                    if (sum > node_count - 1) {
                        _msg::__fail_msg(_msg::_defl,
                            tools::string_format("the sum of sons can't be greater than %d, but found.", node_count - 1, sum)); 
                    }
                    int res = node_count - 1 - sum;
                    if (p != -1 && res == 0) 
                        _msg::__fail_msg(_msg::_defl, "the sons of root node can't be 0.");
                    std::vector<int> rand_sons = rand_array::rand_sum(cnt, res, 0);
                    if (p != -1) {
                        while(rand_sons[p] == 0) {
                            int v = rand_numeric::rand_int(cnt);
                            std::swap(rand_sons[p], rand_sons[v]);
                        }                        
                    }
                    int j = 0;
                    for (auto& s : sons) {
                        if (s < 0) {
                            s = rand_sons[j];
                            j++;                            
                        }
                    }
                }
            };

            template <typename NodeType, typename EdgeType>
            class SonTree : public _GenTree<NodeType, EdgeType> {
            protected:
                using _Self = SonTree<NodeType, EdgeType>;
                _OUTPUT_FUNCTION(_Self)
                _DEF_GEN_FUNCTION
                std::vector<int> _sons;
            public:
                template<typename T = NodeType, typename U = EdgeType, _IsBothWeight<T, U> = 0>
                SonTree(int node_count = 1, int begin_node = 1, int root = 1,
                    NodeGenFunction nodes_weight_function = nullptr,
                    EdgeGenFunction edges_weight_function = nullptr) :
                    _GenTree<NodeType, EdgeType>(node_count, begin_node, true, root, nodes_weight_function, edges_weight_function)
                {
                    _TREE_GRAPH_DEFAULT
                }

                template<typename T = NodeType, typename U = EdgeType, _IsEdgeWeight<T, U> = 0>
                SonTree(int node_count = 1, int begin_node = 1, int root = 1,
                    EdgeGenFunction edges_weight_function = nullptr) :
                    _GenTree<NodeType, EdgeType>(node_count, begin_node, true, root, edges_weight_function)
                {
                    _TREE_GRAPH_DEFAULT
                }

                template<typename T = NodeType, typename U = EdgeType, _IsNodeWeight<T, U> = 0>
                SonTree(int node_count = 1, int begin_node = 1, int root = 1,
                    NodeGenFunction nodes_weight_function = nullptr) :
                    _GenTree<NodeType, EdgeType>(node_count, begin_node, true, root, nodes_weight_function)
                {
                    _TREE_GRAPH_DEFAULT
                }

                template<typename T = NodeType, typename U = EdgeType, _IsUnweight<T, U> = 0>
                SonTree(int node_count = 1, int begin_node = 1, int root = 1) :
                    _GenTree<NodeType, EdgeType>(node_count, begin_node, true, root)
                {
                    _TREE_GRAPH_DEFAULT
                }

                _OUTPUT_FUNCTION_SETTING(_Self)

                _SET_GET_VALUE(std::vector<int>, sons)

                void set_sons(int index, int son) {
                    if (index <= 0 && index > this->_node_count) {
                        _msg::__set_fail_msg(_msg::_defl, 
                            tools::string_format("the index should in range [1, %d], but found %d.", this->_node_count, index));
                        return;
                    }
                    if (_sons.size() < index) _sons.resize(this->_node_count, -1);
                    _sons[index - 1] = son; 
                }
            protected:

                _DEFAULT_GRAPH_GEN_FUNC(SonTree)
            };
        } // namespace basic
    } // namespace rand_graph
} // namespace generator

#endif // !_SGPCET_SON_TREE_H_
