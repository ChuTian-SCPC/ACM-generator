#ifndef _SGPCET_HEIGHT_TREE_H_
#define _SGPCET_HEIGHT_TREE_H_

#ifndef _SGPCET_TREE_H_
#include "tree.h"
#endif // !_SGPCET_TREE_H_

namespace generator {
    namespace rand_graph {
        namespace basic {
            template<typename NodeType, typename EdgeType>
            class HeightTree;

            template <typename NodeType, typename EdgeType>
            class HeightTreeGen : public BasicRandomFatherGen<HeightTree, NodeType, EdgeType> {
            protected:
                using Context = HeightTree<NodeType, EdgeType>;
            public:
                HeightTreeGen(Context& tree) : BasicRandomFatherGen<HeightTree, NodeType, EdgeType>(tree) {}
            protected:
                virtual void __self_init() override {
                    _CONTEXT_GET_REF(int, height)
                    _CONTEXT_GET(int, node_count)
                    if (height == -1) {
                        height = rnd.next(node_count == 1 ? 1 : 2, node_count);
                    }
                }

                virtual void __judge_self_limit() override{
                    _CONTEXT_GET(int, node_count)
                    _CONTEXT_GET(int, height)
                    if (height > node_count || (node_count > 1 && height <= 1) || height < 1) {
                        _msg::__fail_msg(_msg::_defl, 
                            tools::string_format("restriction of the height is [%d,%d].", 
                            node_count == 1 ? 1 : 2, node_count));
                    }
                }

                virtual void __random_father() override {
                    _CONTEXT_GET(int, height)
                    _CONTEXT_GET(int, node_count)
                    std::vector<int> number(height, 1);
                    int w = node_count - height;
                    for (int i = 1; i <= w; i++) {
                        number[rnd.next(1, height - 1)]++;
                    }
                    int l = 0, r = 0, k = 0;
                    for (int i = 1; i < node_count; i++) {
                        if (r + number[k] == i) {
                            l = r;
                            r += number[k];
                            k++;
                        }
                        int f = rnd.next(l, r - 1);
                        this->__add_edge(this->_rank[f], this->_rank[i]);
                    }
                }
            };

            template <typename NodeType, typename EdgeType>
            class HeightTree : public _GenTree<NodeType, EdgeType> {
            protected:
                using _Self =  HeightTree<NodeType,EdgeType>;
                _OUTPUT_FUNCTION(_Self)
                _DEF_GEN_FUNCTION
                int _height;
            public:
                template<typename T = NodeType, typename U = EdgeType, _IsBothWeight<T, U> = 0>
                HeightTree(int node = 1, int begin_node = 1, int root = 1, int height = -1,
                    NodeGenFunction nodes_weight_function = nullptr,
                    EdgeGenFunction edges_weight_function = nullptr) :
                    _GenTree<NodeType, EdgeType>(node, begin_node, true, root,
                        nodes_weight_function, edges_weight_function),
                    _height(height)
                {
                    _TREE_GRAPH_DEFAULT       
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsEdgeWeight<T, U> = 0>
                HeightTree(int node = 1, int begin_node = 1,  int root = 1, int height = -1,
                    EdgeGenFunction edges_weight_function = nullptr) :
                    _GenTree<NodeType, EdgeType>(node, begin_node, true, root, edges_weight_function),
                    _height(height)
                {
                    _TREE_GRAPH_DEFAULT        
                }

                template<typename T = NodeType, typename U = EdgeType, _IsNodeWeight<T, U> = 0>
                HeightTree(int node = 1, int begin_node = 1,  int root = 1, int height = -1,
                    NodeGenFunction nodes_weight_function = nullptr) :
                    _GenTree<NodeType, EdgeType>(node, begin_node, true, root, nodes_weight_function),
                    _height(height)
                {
                    _TREE_GRAPH_DEFAULT        
                }
                
                template<typename T = NodeType, typename U = EdgeType, _IsUnweight<T, U> = 0>
                HeightTree(int node = 1, int begin_node = 1, int root = 1, int height = -1) :
                    _GenTree<NodeType, EdgeType>(node, begin_node, true, root),
                    _height(height)
                {
                    _TREE_GRAPH_DEFAULT       
                }  
                _OUTPUT_FUNCTION_SETTING(_Self)

                _SET_GET_VALUE(int, height)
                _MUST_IS_ROOTED
            protected:
                _DEFAULT_GEN_FUNC(HeightTree)
            };   
        } // namespace basic
    } // namespace rand_graph
} // namespace generator

#endif // !_SGPCET_HEIGHT_TREE_H_
