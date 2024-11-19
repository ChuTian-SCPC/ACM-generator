#ifndef _SGPCET_BASIC_TREE_H_
#define _SGPCET_BASIC_TREE_H_

#ifndef _SGPCET_NODE_H_
#include "node.h"
#endif // !_SGPCET_NODE_H_
#ifndef _SGPCET_EDGE_H_
#include "edge.h"
#endif // !_SGPCET_NODE_H_
#ifndef _SGPCET_LOGGER_H_
#include "log/logger.h"
#endif // !_SGPCET_NODE_H_

namespace generator {
    namespace rand_graph {
        namespace basic {
            class _BasicTree {
            protected:
                int _node_count; // the number of nodes in the tree  
                int _begin_node; // index of the first node                   
                bool _is_rooted;
                // use if `_is_rooted` is true,
                int _root;
                
                // output format
                bool _output_node_count;
                bool _output_root;
                bool _swap_node;// true means output `father son` or `son father` by random

                std::vector<int> _node_indices;
                
            public:
                _BasicTree(
                    int node_count, int begin_node, bool is_rooted, int root,
                    bool output_node_count, bool output_root) :
                    _node_count(node_count),
                    _begin_node(begin_node),
                    _is_rooted(is_rooted),
                    _root(root - 1),
                    _output_node_count(output_node_count),
                    _output_root(output_root),
                    _swap_node(_is_rooted ? false : true)
                {
                    __init_node_indices();
                }
                
                virtual ~_BasicTree() = default;

                void set_node_count(int node_count) { 
                    if (node_count != _node_count) {
                        _node_count = node_count; 
                        __init_node_indices();               
                    }             
                }
                _GET_VALUE(int, node_count)

                void set_is_rooted(int is_rooted) { 
                    if (_is_rooted != is_rooted) {
                        _swap_node = is_rooted ? false : true;
                        _msg::__warn_msg(_msg::_defl, 
                            tools::string_format("setting `swap_node` to %s, becase `is_rooted` changed!", 
                            (_swap_node ? "true" : "false")));
                    }
                    _is_rooted = is_rooted; 
                }
                _GET_VALUE(bool, is_rooted)

                void set_root(int root) {
                    _root = root - 1;
                    if (!_is_rooted) {
                       _msg::__warn_msg(_msg::_defl, "unrooted Tree, set root is useless."); 
                    }
                }
                int root() const {
                    if (!_is_rooted) {
                        _msg::__warn_msg(_msg::_defl, "unrooted Tree, root is useless.");
                    }
                    return _node_indices[_root];
                }
                
                int& root_ref() {
                    if (!_is_rooted) {
                        _msg::__warn_msg(_msg::_defl, "unrooted Tree, root is useless.");
                    }
                    return _root;
                }

                void set_begin_node(int begin_node) { 
                    if (begin_node != _begin_node) {
                        _begin_node = begin_node; 
                        __init_node_indices();                 
                    }                 
                }
                _GET_VALUE(int, begin_node)

                _SET_GET_VALUE(bool, output_node_count)
                _SET_GET_VALUE(bool, output_root)
                _SET_GET_VALUE(bool, swap_node)
                
                void set_node_indices(std::vector<int> node_indices) {
                    if ((int)node_indices.size() != _node_count) {
                        _msg::__warn_msg(_msg::_defl,
                            tools::string_format("size of node_indices must equal to node_count %d, but found %d.", 
                            _node_count, node_indices.size()));
                        return;
                    }
                    _node_indices = node_indices;
                }         
                void set_node_indices(int index, int number) {
                    if (index < 1 || index > _node_count) {
                        _msg::__warn_msg(_msg::_defl,
                            tools::string_format("node index must in range [1, %d], but found %d.",
                            _node_count, index));
                        return;
                    }
                    _node_indices[index - 1] = number;
                }
                _GET_VALUE(std::vector<int>, node_indices)
                void default_node_indices() { __init_node_indices(); }
                
            protected:
                
                void __init_node_indices() {
                    _node_indices.clear();
                    for (int i = 0 ; i < _node_count; i++) {
                        _node_indices.emplace_back(i + _begin_node);
                    }
                }
            }; 

        } // namespace basic
    } // namespace rand_graph
} // namespace generator

#endif // !_SGPCET_BASIC_TREE_H_
