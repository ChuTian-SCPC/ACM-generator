#ifndef _SGPCET_NODE_WEIGHT_H_
#define _SGPCET_NODE_WEIGHT_H_

#ifndef _SGPCET_TREE_H_
#include "tree.h"
#endif // !_SGPCET_TREE_H_
#ifndef _SGPCET_CHAIN_H_
#include "chain.h"
#endif // !_SGPCET_CHAIN_H_
#ifndef _SGPCET_FLOWER_H_
#include "flower.h"
#endif // !_SGPCET_FLOWER_H_
#ifndef _SGPCET_HEIGHT_TREE_H_
#include "graph/height_tree.h"
#endif // !_SGPCET_HEIGHT_TREE_H_

namespace generator {
    namespace rand_graph {
        namespace node_weight {
            using Edge = basic::_Edge<void>;

            template <typename NodeType>
            using NodeWeight = basic::_Node<NodeType>;

            using TreeGenerator = _enum::TreeGenerator;

            template <typename NodeType>
            using TreeGen = basic::TreeGen<NodeType, void>;

            template <typename NodeType>
            using RandomFatherGen = basic::RandomFatherGen<NodeType, void>;

            template <typename NodeType>
            using PrueferGen = basic::PrueferGen<NodeType, void>;

            template <typename NodeType>
            using Tree = basic::Tree<NodeType, void>;

            template <typename NodeType>
            using ChainGen = basic::ChainGen<NodeType, void>;

            template <typename NodeType>
            using Chain = basic::Chain<NodeType, void>;

            template <typename NodeType>
            using FlowerGen = basic::FlowerGen<NodeType, void>;

            template <typename NodeType>
            using Flower = basic::Flower<NodeType, void>;
        } // namespace node_weight
    } // namespace rand_graph
} // namespace generator

#endif // !_SGPCET_NODE_WEIGHT_H_
