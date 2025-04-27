#ifndef _SGPCET_LINK_FORWARD_H_
#define _SGPCET_LINK_FORWARD_H_

#ifndef _SGPCET_COMMON_H_
#include "basic/common.h"
#endif // !_SGPCET_COMMON_H_

namespace generator {
    namespace rand_graph {
        namespace basic {
            template<typename NodeType, typename EdgeType> 
            class _LinkImpl;
            
            template<typename NodeType, typename EdgeType>
            class _TreeLinkImpl;

        } // namespace basic
    } // namespace rand_graph
} // namespace generator

#endif // !_SGPCET_LINK_FORWARD_H_
