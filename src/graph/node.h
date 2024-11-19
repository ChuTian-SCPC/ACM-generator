#ifndef _SGPCET_NODE_H_
#define _SGPCET_NODE_H_

#ifndef _SGPCET_MACRO_H_
#include "basic/macro.h"
#endif // !_SGPCET_MACRO_H_

namespace generator {
    namespace rand_graph {
        namespace basic {
            template<typename U>
            class _Node {
            protected:
                using _Self = _Node<U>;
                U _w;
                _OUTPUT_FUNCTION(_Self)
            public:
                _Node() : _w(U()) {
                    _output_function = default_function();
                }
                _Node(U w) : _w(w) {
                    _output_function = default_function();
                }
                
                _SET_GET_VALUE(U, w)
                
                void default_output(std::ostream& os) const {
                    os << _w ;
                }

                _OUTPUT_FUNCTION_SETTING(_Self)
            };
            
            template<>
            class _Node<void> {
            protected:
                using _Self = _Node<void>;
                _OUTPUT_FUNCTION(_Self)
            public:
                _Node(){
                    _output_function = default_function();
                }

                void default_output(std::ostream& os) const { return; }

                _OUTPUT_FUNCTION_SETTING(_Self)
            };
        } // namespace basic
    } // namespace rand_graph
} // namespace generator

#endif // !_SGPCET_NODE_H_
