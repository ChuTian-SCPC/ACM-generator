#ifndef _SGPCET_BASIC_TREE_GRAPH_H_
#define _SGPCET_BASIC_TREE_GRAPH_H_

#ifndef _SGPCET_NODE_H_
#include "node.h"
#endif // !_SGPCET_NODE_H_
#ifndef _SGPCET_EDGE_H_
#include "edge.h"
#endif // !_SGPCET_NODE_H_
#ifndef _SGPCET_LOGGER_H_
#include "log/logger.h"
#endif // !_SGPCET_NODE_H_
#ifndef _SGPCET_NUMERIC_H_
#include "rand/numeric.h"
#endif // !_SGPCET_NUMERIC_H_

namespace generator {
    namespace rand_graph {
        namespace basic {
            class _BasicTreeGraph {
            protected:
                int _node_count; // the number of nodes in the tree  
                int _begin_node; // index of the first node 

                // output format
                bool _output_node_count;
                bool _swap_node;// true means output `father son` or `son father` by random

                std::vector<int> _node_indices;
                bool _node_indices_changed;
            public:
                _BasicTreeGraph(int node_count, int begin_node, bool output_node_count, bool swap_node):
                    _node_count(node_count),
                    _begin_node(begin_node),
                    _output_node_count(output_node_count),
                    _swap_node(swap_node),
                    _node_indices_changed(false)
                {
                    __init_node_indices();
                }

                virtual ~_BasicTreeGraph() = default;

                void set_node_count(int node_count) { 
                    if (node_count != _node_count) {
                        if (_node_indices_changed) {
                            _msg::__warn_msg(_msg::_defl, 
                                tools::string_format("using default `node_indices` for `node_count` changed (%d -> %d).",
                                _node_count, node_count));                            
                        }
                        _node_count = node_count; 
                        __init_node_indices();               
                    }             
                }
                _GET_VALUE(int, node_count)

                void set_begin_node(int begin_node) { 
                    if (begin_node != _begin_node) {
                        if (_node_indices_changed) {
                            _msg::__warn_msg(_msg::_defl, 
                                tools::string_format("using default `node_indices` for `begin_node` changed (%d -> %d).",
                                _begin_node, begin_node));                            
                        }

                        _begin_node = begin_node;
                        __init_node_indices();                 
                    }                 
                }
                _GET_VALUE(int, begin_node)

                _SET_GET_VALUE(bool, output_node_count)
                _SET_GET_VALUE(bool, swap_node)

                void set_node_indices(std::vector<int> node_indices) {
                    if ((int)node_indices.size() != _node_count) {
                        _msg::__set_fail_msg(_msg::_defl,
                            tools::string_format("size of node_indices must equal to node_count %d, but found %d.", 
                            _node_count, node_indices.size()));
                        return;
                    }
                    _node_indices_changed = true;
                    _node_indices = node_indices;
                }         
                void set_node_indices(int index, int number) {
                    if (index < 1 || index > _node_count) {
                        _msg::__set_fail_msg(_msg::_defl,
                            tools::string_format("node index must in range [1, %d], but found %d.",
                            _node_count, index));
                        return;
                    }
                    if (_node_indices.size() != _node_count) __init_node_indices();
                    _node_indices_changed = true;
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
                    _node_indices_changed = false;
                }
            };
            
            class _BasicTree : public _BasicTreeGraph {
            protected:
                
                bool _is_rooted;
                // use if `_is_rooted` is true,
                int _root;
                bool _output_root;    
            public:
                _BasicTree(
                    int node_count, int begin_node, bool is_rooted, int root,
                    bool output_node_count, bool output_root) :
                    _BasicTreeGraph(node_count, begin_node, output_node_count, !is_rooted),
                    _is_rooted(is_rooted),
                    _root(root - 1),
                    _output_root(output_root) {}

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

                _SET_GET_VALUE(bool, output_root)
                
            }; 

            class _BasicGraph : public _BasicTreeGraph {
            protected:
                int _edge_count;
                bool _direction;
                bool _multiply_edge;
                bool _self_loop;
                bool _connect;
                bool _output_edge_count; 
            
            public:

                _BasicGraph(int node_count, int edge_count, int begin_node,
                    bool direction, bool multiply_edge, bool self_loop, bool connect,
                    bool swap_node, bool output_node_count, bool output_edge_count) :
                    _BasicTreeGraph(node_count, begin_node, output_node_count, swap_node),
                    _edge_count(edge_count),
                    _direction(direction),
                    _multiply_edge(multiply_edge),
                    _self_loop(self_loop),
                    _connect(connect),
                    _output_edge_count(output_edge_count) {}
                
                _SET_GET_VALUE(int, edge_count)

                void set_direction(bool direction) { 
                    if (_direction != direction) {
                        _swap_node = direction ? false : true;
                        _msg::__warn_msg(_msg::_defl, 
                            tools::string_format("setting `swap_node` to %s, becase `direction` changed!",
                            _swap_node ? "true" : "false"));
                    }
                    _direction = direction; 
                }
                _GET_VALUE(bool, direction)

                _SET_GET_VALUE(bool, multiply_edge)
                _SET_GET_VALUE(bool, self_loop)
                _SET_GET_VALUE(bool, connect)
                _SET_GET_VALUE(bool, output_edge_count)

                virtual long long max_edge_count() {
                    if (this->_node_count == 0) return 0;
                    long long sum = 0;
                    if (this->_multiply_edge) {
                        if (this->_node_count == 1 && !this->_self_loop) return 0;
                        return _setting::_edge_count_inf;
                    } else {
                        long long n = (long long)this->_node_count;
                        sum += n * (n - 1) / 2;
                        if (this->_direction) sum *= 2;
                        if (this->_self_loop) sum += n;
                        return sum;
                    }
                };

                virtual long long min_edge_count() {
                    if (this->_node_count == 0) return 0;
                    return this->_connect ? this->_node_count - 1 : 0;
                };

                virtual void rand_edge_count(long long from = _setting::_auto_edge_limit, long long to = _setting::_auto_edge_limit) {
                    auto p = __edge_from_to_limit(from, to);
                    _edge_count = rand_numeric::rand_int(p.first, p.second); 
                }
            protected:
                std::pair<long long, long long> __edge_from_to_limit(long long from, long long to) {
                    long long l = min_edge_count();
                    long long r = max_edge_count();
                    long long limit = _setting::edge_limit;
                    if (r > limit || r == _setting::_edge_count_inf) {
                        r = limit;
                    }                    
                    if (from != _setting::_auto_edge_limit) l = std::max(l, from);
                    if (to != _setting::_auto_edge_limit) r = std::min(r, to);
                    return std::make_pair(l, r);
                }
            };

        } // namespace basic
    } // namespace rand_graph
} // namespace generator

#endif // !_SGPCET_BASIC_TREE_GRAPH_H_
