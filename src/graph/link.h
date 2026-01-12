#ifndef _SGPCET_LINK_H_
#define _SGPCET_LINK_H_

#ifndef _SGPCET_TREE_H_
#include "tree.h"
#endif // !_SGPCET_TREE_H_
#ifndef _SGPCET_GRAPH_H_
#include "graph.h"
#endif // !_SGPCET_GRAPH_H_

namespace generator {
    namespace rand_graph {
        namespace basic {
            template <typename U>
            struct IsTreeOrGraph {
                template <typename V>
                static constexpr auto check(V *)
                -> decltype(std::declval<V>().edges(), std::true_type());

                template <typename V>
                static constexpr std::false_type check(...);

                static constexpr bool value =
                        decltype(check<U>(nullptr))::value;
            };

            template <typename NodeType, typename EdgeType>
            class Link;

            template <typename NodeType, typename EdgeType>
            class LinkGen : public BasicGraphGen<Link, NodeType, EdgeType> {
            protected:
                using Context = Link<NodeType, EdgeType>;
                std::vector<std::vector<_Edge<EdgeType>>> _source_edges;
                std::vector<std::vector<_Node<NodeType>>> _source_nodes_weight;
                std::vector<std::vector<int>> _source_node_indices;
                int _source_count;
                std::vector<int> _father;
                std::map<int, std::vector<int>> _connect_parts;
                std::map<std::pair<int, int>, int> _node_merge_map;
                std::vector<int> _source_node_count;
            public:
                LinkGen(Context& graph) : BasicGraphGen<Link, NodeType, EdgeType>(graph), _source_count(0) {}
                
                template<template<typename, typename> class TG>
                void set_target(TG<NodeType, EdgeType>& target) {
                    this->__set_target(target);
                }

                template<template<typename, typename> class TG>
                void add_source(TG<NodeType, EdgeType>& source) {
                    this->__add_source(source);
                }

                virtual void generate() override {
                    this->__merge_source();
                    if (_CONTEXT_V(extra_edges_count)) this->_context.check_edges_weight_function();
                    this->__judge_limits();
                    this->__generate_graph(); 
                    _CONTEXT_GET_REF(edges)
                    shuffle(edges.begin(), edges.end());
                };
            protected:
                template<template<typename, typename> class TG, typename T = NodeType, _HasT<T> = 0>
                void __reset_nodes_weight_function(TG<NodeType, EdgeType>& graph) {
                    auto func = graph.nodes_weight_function();
                    this->_context.set_nodes_weight_function(func);
                }
                
                template<template<typename, typename> class TG, typename T = NodeType, _NotHasT<T> = 0>
                void __reset_nodes_weight_function(TG<NodeType, EdgeType>&) {
                    return;
                }
                
                template<template<typename, typename> class TG, typename T = EdgeType, _HasT<T> = 0>
                void __reset_edges_weight_function(TG<NodeType, EdgeType>& graph) {
                    auto func = graph.edges_weight_function();
                    this->_context.set_edges_weight_function(func);
                }
                
                template<template<typename, typename> class TG, typename T = EdgeType, _NotHasT<T> = 0>
                void __reset_edges_weight_function(TG<NodeType, EdgeType>&) {
                    return;
                }

                void __set_target(_GenGraph<NodeType, EdgeType>& target) {
                   _CONTEXT_V_REF(direction) = target.direction();
                   _CONTEXT_V_REF(connect) = target.connect();
                   _CONTEXT_V_REF(multiply_edge) = target.multiply_edge();
                   _CONTEXT_V_REF(self_loop) = target.self_loop();
                   _CONTEXT_V_REF(output_edge_count) = target.output_edge_count();
                   this->__set_target_common(target);
                }

                void __set_target(_GenTree<NodeType, EdgeType>& target) {
                    _CONTEXT_V_REF(direction) = target.is_rooted();
                    this->__set_target_common(target);
                }

                template<template<typename, typename> class TG>
                void __set_target_common(TG<NodeType, EdgeType>& target) {
                    _CONTEXT_V_REF(swap_node) = target.swap_node();
                    _CONTEXT_V_REF(begin_node) = target.begin_node();
                    _CONTEXT_V_REF(output_node_count) = target.output_node_count();
                    this->__reset_nodes_weight_function(target);
                    this->__reset_edges_weight_function(target);
                }

                template<template<typename, typename> class TG, typename T = NodeType, _HasT<T> = 0>
                void __add_source_nodes_weight(TG<NodeType, EdgeType>& source) {
                    _source_nodes_weight.emplace_back(source.nodes_weight());
                }


                template<template<typename, typename> class TG, typename T = NodeType, _NotHasT<T> = 0>
                void __add_source_nodes_weight(TG<NodeType, EdgeType>&) {
                    return;
                }

                void __init_source(_GenGraph<NodeType, EdgeType>& source) {
                    if (!__is_generated(source)) source.gen();
                }

                void __init_source(_GenTree<NodeType, EdgeType>& source) {
                    if (!__is_generated(source)) source.gen();
                }
                
                template<typename T = NodeType, _NotHasT<T> = 0>
                bool __is_generated(_GenTree<NodeType, EdgeType>& source) {
                    if (source.node_count() - 1 != (int)source.edges_ref().size()) return false;
                    return true;
                }

                template<typename T = NodeType, _HasT<T> = 0>
                bool __is_generated(_GenTree<NodeType, EdgeType>& source) {
                    if (source.node_count() - 1 != (int)source.edges_ref().size()) return false;
                    if (source.node_count() != (int)source.nodes_weight_ref().size()) return false;
                    return true;
                }

                template<typename T = NodeType, _NotHasT<T> = 0>
                bool __is_generated(_GenGraph<NodeType, EdgeType>& source) {
                    if (source.edge_count() != (int)source.edges_ref().size()) return false;
                    return true;
                }

                template<typename T = NodeType, _HasT<T> = 0>
                bool __is_generated(_GenGraph<NodeType, EdgeType>& source) {
                    if (source.edge_count() != (int)source.edges_ref().size()) return false;
                    if (source.node_count() != (int)source.nodes_weight_ref().size()) return false;
                    return true;
                }

                template<template<typename, typename> class TG>
                void __add_source(TG<NodeType, EdgeType>& source) {
                    __init_source(source);
                    _source_edges.emplace_back(source.edges_ref());
                    _source_node_count.emplace_back(source.node_count());
                    _source_node_indices.emplace_back(source.node_indices());
                    __add_source_nodes_weight(source);
                    _source_count++;
                }

                void __generate_connect_part() {
                    std::vector<int> mark_indices;
                    for (auto it : _connect_parts) {
                        mark_indices.emplace_back(it.first);
                    }
                    Tree<void, void> tree(_connect_parts.size(), 0);
                    tree.gen();
                    std::vector<_Edge<void>> edges = tree.edges();
                    for (_Edge<void> edge : edges) {
                        int u = rnd.any(_connect_parts[mark_indices[edge.u()]]);
                        int v = rnd.any(_connect_parts[mark_indices[edge.v()]]);
                        this->__add_edge(u, v);
                    }
                }

                virtual void __generate_graph() override {
                    int m = _CONTEXT_V(extra_edges_count);
                    if (_CONTEXT_V(connect)) {
                        m -= _connect_parts.size() - 1;
                        __generate_connect_part();
                    }
                    while (m--){
                        this->__add_edge(this->__rand_edge());
                    }    
                }

                void __merge_source() {
                    __merge_node_indices();
                    __reset_node_count();
                    __merge_nodes_weight();
                    __merge_edges();
                    __reset_edge_count();
                    __divide_connection_part();
                }

                void __merge_node_indices() {
                    _node_merge_map.clear();    
                    _CONTEXT_GET(link_type); 
                    _CONTEXT_GET_REF(node_indices);
                    node_indices.clear();
                    if (link_type == _enum::LinkType::Dedupe) {
                        std::map<int, int> first_appear;
                        int cnt = 0;
                        for (int i = 0; i < _source_count; i++) {
                            for (int j = 0; j < _source_node_count[i]; j++ ) {
                                int x = _source_node_indices[i][j];
                                if (first_appear.find(x) == first_appear.end()) {
                                    node_indices.emplace_back(x);
                                    first_appear[x] = cnt;
                                    cnt++;
                                }
                                _node_merge_map[std::make_pair(i, j)] = first_appear[x];
                            }
                        }
                    }
                    else {
                        std::vector<int> p;
                        int cnt = 0;
                        for (int i = 0; i < _source_count; i++) {
                            for (int j = 0; j < _source_node_count[i]; j++) {
                                p.emplace_back(cnt);
                                cnt++;
                            }                            
                        }
                        if (link_type == _enum::LinkType::Shuffle) shuffle(p.begin(), p.end());
                        cnt = 0;
                        for (int i = 0; i < _source_count; i++) {
                            for (int j = 0; j < _source_node_count[i]; j++) {
                                _node_merge_map[std::make_pair(i, j)] = p[cnt];
                                cnt++;
                            }
                        }               
                        
                        if (link_type == _enum::LinkType::Direct) {
                            for (int i = 0; i < _source_count; i++) {
                                for (auto x : _source_node_indices[i]) {
                                    node_indices.emplace_back(x);
                                }
                            }
                        }
                        else {
                            cnt = _CONTEXT_V(begin_node);
                            for (int i = 0; i < _source_count; i++) {
                                for (int j = 0; j < _source_node_count[i]; j++) {
                                    node_indices.emplace_back(cnt);
                                    cnt++;
                                }
                            }
                        }
                    }                
                }   

                void __reset_node_count() {
                    _CONTEXT_V_REF(node_count) = _CONTEXT_V(node_indices).size();
                }   

                template<typename T = NodeType, _HasT<T> = 0>
                void __merge_nodes_weight() {
                    _CONTEXT_GET(link_type);
                    _CONTEXT_GET_REF(nodes_weight);
                    nodes_weight.clear();
                    if (link_type == _enum::LinkType::Dedupe) {
                        std::set<int> appear;
                        nodes_weight.resize(_CONTEXT_V(node_count));
                        for (auto& it : _node_merge_map) {
                            if (appear.find(it.second) == appear.end()) {
                                appear.insert(it.second);
                                nodes_weight[it.second] = _source_nodes_weight[it.first.first][it.first.second];
                            }
                        }
                    }
                    else {
                        nodes_weight.resize(_CONTEXT_V(node_count));
                        for (auto& it : _node_merge_map) {
                            nodes_weight[it.second] = _source_nodes_weight[it.first.first][it.first.second];
                        }
                    }
                }

                template<typename T = NodeType, _NotHasT<T> = 0>
                void __merge_nodes_weight() {
                    return;
                }

                void __merge_edges() {
                    _CONTEXT_GET_REF(edges);
                    edges.clear();
                    if (!_CONTEXT_V(multiply_edge)) this->_e.clear();
                    int ignore_edges = 0;
                    for (int i = 0; i < _source_count; i++) {
                        int sz = _source_edges[i].size();
                        for (int j = 0; j < sz; j++) {
                            _Edge<EdgeType> edge = _source_edges[i][j];
                            int& u = edge.u_ref();
                            int& v = edge.v_ref();
                            u = _node_merge_map[std::make_pair(i, u)];
                            v = _node_merge_map[std::make_pair(i, v)];
                            if (this->__judge_multiply_edge(u, v) || this->__judge_self_loop(u, v)) ignore_edges++;
                            else this->__add_edge(edge);
                        }
                    }
                    if (ignore_edges) {
                        _msg::__warn_msg(_msg::_defl, 
                            tools::string_format("ignore %d edge(s) due to the graph's attribute-based conditions.", ignore_edges));
                    }
                }

                void __reset_edge_count() {
                    _CONTEXT_V_REF(edge_count) = _CONTEXT_V(edges).size() + _CONTEXT_V(extra_edges_count);
                }    

                int __find(int x) {
                    if (_father[x] != x) {
                        _father[x] = __find(_father[x]);
                    }
                    return _father[x];
                }

                void __divide_connection_part() {
                    if (!_CONTEXT_V(connect)) { return; }
                    _father.clear();
                    _connect_parts.clear();
                    _CONTEXT_GET(node_count);
                    for (int i = 0; i < node_count; i++) {
                        _father.emplace_back(i);
                    }
                    for (auto edge : _CONTEXT_V_REF(edges)) {
                        int u = edge.u();
                        int v = edge.v();
                        int t1 = __find(u);
                        int t2 = __find(v);
                        if (t1 != t2) {
                            _father[t1] = t2;
                        }
                    }
                    for (int i = 0; i < node_count; i++) {
                        _connect_parts[__find(i)].emplace_back(i);
                    }
                }    
            };

            template <typename NodeType, typename EdgeType>
            class Link : public _GenGraph<NodeType, EdgeType> {
            protected:
                using _Self =  Link<NodeType,EdgeType>;
                _OUTPUT_FUNCTION(_Self)
                _DEF_GEN_FUNCTION
                int _extra_edges_count;
                _enum::LinkType _link_type;
            public:

                Link(int extra_edge_count = 0, _enum::LinkType link_type = _enum::LinkType::Shuffle) :
                    _GenGraph<NodeType, EdgeType>(), _extra_edges_count(extra_edge_count), _link_type(link_type) 
                {
                    _TREE_GRAPH_DEFAULT
                }

                template<template<typename, typename> class TG, typename Gen = LinkGen<NodeType, EdgeType>>
                void set_target(TG<NodeType, EdgeType>& target) {
                    (dynamic_cast<Gen*>(this->_generator))->set_target(target);
                }

                template<template<typename, typename> class TG, typename Gen = LinkGen<NodeType, EdgeType>>
                void add_source(TG<NodeType, EdgeType>& source) {
                    (dynamic_cast<Gen*>(this->_generator))->add_source(source);
                }

                _SET_GET_VALUE(int, extra_edges_count);
                _SET_GET_VALUE(_enum::LinkType, link_type);

                _OUTPUT_FUNCTION_SETTING(_Self)
            protected:
                _DEFAULT_GRAPH_GEN_FUNC(Link)
            };    
         
            template <typename NodeType, typename EdgeType>
            class TreeLink;          

            template <typename NodeType, typename EdgeType>
            class TreeLinkGen : public BasicTreeGen<TreeLink, NodeType, EdgeType> {
            protected:
                using Context = TreeLink<NodeType, EdgeType>;
                Link<NodeType, EdgeType> _link;
                int _source_count;   

            public:
                TreeLinkGen(Context& tree) : BasicTreeGen<TreeLink, NodeType, EdgeType>(tree), _link(), _source_count(0) {}
                
                void set_target(_GenTree<NodeType, EdgeType>& target) {
                    __set_target(target);
                }

                void add_source(_GenTree<NodeType, EdgeType>& source) {
                    _link.add_source(source);
                    _source_count++;
                }

                virtual void generate() override {
                    _msg::OutStream graph_log(false);
                    _msg::_defl.swap(graph_log);
                    _link.set_target(this->_context);
                    _link.set_extra_edges_count(_source_count -  1);
                    _link.set_link_type(__convert_to_link_type());
                    _link.set_connect(true);
                    _link.gen();
                    __dump_result();
                    _msg::_defl.swap(graph_log);
                };
            protected:
                template<template<typename, typename> class TG, typename T = NodeType, _HasT<T> = 0>
                void __reset_nodes_weight_function(TG<NodeType, EdgeType>& graph) {
                    auto func = graph.nodes_weight_function();
                    this->_context.set_nodes_weight_function(func);
                }
                
                template<template<typename, typename> class TG, typename T = NodeType, _NotHasT<T> = 0>
                void __reset_nodes_weight_function(TG<NodeType, EdgeType>&) {
                    return;
                }
                
                template<template<typename, typename> class TG, typename T = EdgeType, _HasT<T> = 0>
                void __reset_edges_weight_function(TG<NodeType, EdgeType>& graph) {
                    auto func = graph.edges_weight_function();
                    this->_context.set_edges_weight_function(func);
                }
                
                template<template<typename, typename> class TG, typename T = EdgeType, _NotHasT<T> = 0>
                void __reset_edges_weight_function(TG<NodeType, EdgeType>&) {
                    return;
                }

                template<template<typename, typename> class TG>
                void __set_target_common(TG<NodeType, EdgeType>& target) {
                    _CONTEXT_V_REF(swap_node) = target.swap_node();
                    _CONTEXT_V_REF(begin_node) = target.begin_node();
                    _CONTEXT_V_REF(output_node_count) = target.output_node_count();
                    this->__reset_nodes_weight_function(target);
                    this->__reset_edges_weight_function(target);
                }

                void __set_target(_GenTree<NodeType, EdgeType>& target) {
                    _CONTEXT_V_REF(is_rooted) = target.is_rooted();
                    if (_CONTEXT_V(is_rooted)) _CONTEXT_V_REF(root) = target.root_ref();
                    __set_target_common(target);
                } 

                void __set_target(_GenGraph<NodeType, EdgeType>& target) {
                    _CONTEXT_V_REF(is_rooted) = target.direction();
                    __set_target_common(target);
                } 

                _enum::LinkType __convert_to_link_type() {
                    _CONTEXT_GET(link_type);
                    if (link_type == _enum::TreeLinkType::Direct) return _enum::LinkType::Direct;
                    else if(link_type == _enum::TreeLinkType::Increase) return _enum::LinkType::Increase;
                    return _enum::LinkType::Shuffle;
                }

                template<typename T = NodeType, _NotHasT<T> = 0>
                void __dump_nodes_weight() {
                    return;
                }

                template<typename T = NodeType, _HasT<T> = 0>
                void __dump_nodes_weight() {
                    _CONTEXT_V_REF(nodes_weight) = _link.nodes_weight();
                }               
                void __dump_result() {
                    _CONTEXT_V_REF(node_count) = _link.node_count();
                    _CONTEXT_V_REF(node_indices) = _link.node_indices();
                    _CONTEXT_V_REF(begin_node) = _link.begin_node();
                    __dump_nodes_weight();
                    _CONTEXT_V_REF(edges) = _link.edges_ref();
                }
            };

            template <typename NodeType, typename EdgeType>
            class TreeLink : public _GenTree<NodeType, EdgeType> {
            protected:
                using _Self =  TreeLink<NodeType,EdgeType>;
                _OUTPUT_FUNCTION(_Self)
                _DEF_GEN_FUNCTION
                _enum::TreeLinkType _link_type;
            public:

                TreeLink(_enum::TreeLinkType link_type = _enum::TreeLinkType::Shuffle) :
                    _GenTree<NodeType, EdgeType>(), _link_type(link_type) 
                {
                    _TREE_GRAPH_DEFAULT
                }

                template<template<typename, typename> class TG, typename Gen = TreeLinkGen<NodeType, EdgeType>>
                void set_target(TG<NodeType, EdgeType>& target) {
                    (dynamic_cast<Gen*>(this->_generator))->set_target(target);
                }

                template<template<typename, typename> class TG, typename Gen = TreeLinkGen<NodeType, EdgeType>>
                void add_source(TG<NodeType, EdgeType>& source) {
                    (dynamic_cast<Gen*>(this->_generator))->add_source(source);
                }


                _SET_GET_VALUE(_enum::TreeLinkType, link_type);

                _OUTPUT_FUNCTION_SETTING(_Self)
            protected:
                _DEFAULT_GRAPH_GEN_FUNC(TreeLink)
            };

        } // namespace basic
    } // namespace rand_graph
} // namespace generator

#endif // !_SGPCET_LINK_H_
