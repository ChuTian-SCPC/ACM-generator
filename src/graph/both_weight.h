#ifndef _SGPCET_BOTH_WEIGHT_H_
#define _SGPCET_BOTH_WEIGHT_H_

#ifndef _SGPCET_TREE_H_
#include "tree.h"
#endif // !_SGPCET_TREE_H_
#ifndef _SGPCET_CHAIN_H_
#include "chain.h"
#endif // !_SGPCET_CHAIN_H_

namespace generator {
    namespace rand_graph {
        namespace both_weight {
            template <typename EdgeType>
            using Edge = basic::_Edge<EdgeType>;

            template <typename NodeType>
            using NodeWeight = basic::_Node<NodeType>;

            using TreeGenerator = _enum::TreeGenerator;

            template <typename NodeType, typename EdgeType>
            using TreeGen = basic::TreeGen<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using RandomFatherGen = basic::RandomFatherGen<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using PrueferGen = basic::PrueferGen<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using Tree = basic::Tree<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using ChainGen = basic::ChainGen<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using Chain = basic::Chain<NodeType, EdgeType>;
        } // namespace both_weight
    } // namespace rand_graph
} // namespace generator

#endif // !_SGPCET_BOTH_WEIGHT_H_
