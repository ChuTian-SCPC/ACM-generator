#ifndef _SGPCET_GEN_FUNCTION_H_
#define _SGPCET_GEN_FUNCTION_H_

#ifndef _SGPCET_COMMON_H_
#include "basic/common.h"
#endif // !_SGPCET_COMMON_H_
#ifndef _SGPCET_LOGGER_H_
#include "log/logger.h"
#endif // !_SGPCET_LOGGER_H_
#ifndef _SGPCET_WEIGHT_TYPE_H_
#include "weight_type.h"
#endif // !_SGPCET_WEIGHT_TYPE_H_

namespace generator {
    namespace rand_graph {
        namespace basic {
            template<typename NodeType, typename EdgeType>
            class _RandomFunction {
            protected:
                _DEF_GEN_FUNCTION
                NodeGenFunction _nodes_weight_function;
                EdgeGenFunction _edges_weight_function;
            public:
                _RandomFunction(
                    NodeGenFunction nodes_weight_function,
                    EdgeGenFunction edges_weight_function) :
                    _nodes_weight_function(nodes_weight_function),
                    _edges_weight_function(edges_weight_function) 
                {}
                
                virtual ~_RandomFunction() = default;

                template<typename T = NodeType, _HasT<T> = 0>
                void set_nodes_weight_function(NodeGenFunction nodes_weight_function) {
                    _nodes_weight_function = nodes_weight_function;
                }
                
                template<typename T = EdgeType, _HasT<T> = 0>
                void set_edges_weight_function(EdgeGenFunction edges_weight_function) {
                    _edges_weight_function = edges_weight_function;
                }
                
                _GET_VALUE(NodeGenFunction, nodes_weight_function)
                _GET_VALUE(EdgeGenFunction, edges_weight_function)

                void check_gen_function() {
                    __check_nodes_weight_function();
                    __check_edges_weight_function();
                }

                void check_nodes_weight_function() {
                    __check_nodes_weight_function();
                }

                void check_edges_weight_function() {
                    __check_edges_weight_function();
                }
            protected:
                                                           
                template<typename T = NodeType, _HasT<T> = 0>
                void __check_nodes_weight_function() {
                    if (_nodes_weight_function == nullptr) {
                        _msg::__fail_msg(_msg::_defl, "nodes weight generator function is nullptr, please set it.");
                    }
                }
                
                template<typename T = NodeType, _NotHasT<T> = 0>
                void __check_nodes_weight_function() {}
                
                template<typename T = EdgeType, _HasT<T> = 0>
                void __check_edges_weight_function() {
                    if (_edges_weight_function == nullptr) {
                        _msg::__fail_msg(_msg::_defl, "edges weight generator function is nullptr, please set it.");
                    }
                }
                
                template<typename T = EdgeType, _NotHasT<T> = 0>
                void __check_edges_weight_function() {}
            };

        } // namespace basic
    } // namespace rand_graph
} // namespace generator

#endif // !_SGPCET_GEN_FUNCTION_H_
