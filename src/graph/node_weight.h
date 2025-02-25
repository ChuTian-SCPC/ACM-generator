#ifndef _SGPCET_NODE_WEIGHT_H_
#define _SGPCET_NODE_WEIGHT_H_

#ifndef _SGPCET_ALL_TREE_GRAPH_H_
#include "all_tree_graph.h"
#endif // !_SGPCET_ALL_TREE_GRAPH_H_

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

            template <typename NodeType>
            using HeightTreeGen = basic::HeightTreeGen<NodeType, void>;

            template <typename NodeType>
            using HeightTree = basic::HeightTree<NodeType, void>;

            template <typename NodeType>
            using DegreeTreeGen = basic::DegreeTreeGen<NodeType, void>;

            template <typename NodeType>
            using MaxDegreeTree = basic::MaxDegreeTree<NodeType, void>;

            template <typename NodeType>
            using SonTreeGen = basic::SonTreeGen<NodeType, void>;

            template <typename NodeType>
            using MaxSonTree = basic::MaxSonTree<NodeType, void>;

            template <typename NodeType>
            using GraphGen = basic::GraphGen<NodeType, void>;

            template <typename NodeType>
            using Graph = basic::Graph<NodeType, void>;

            template <typename NodeType>
            using BipartiteGraphGen = basic::BipartiteGraphGen<NodeType, void>;

            template <typename NodeType>
            using BipartiteGraph = basic::BipartiteGraph<NodeType, void>;

            template <typename NodeType>
            using DAGGen = basic::DAGGen<NodeType, void>;

            template <typename NodeType>
            using DAG = basic::DAG<NodeType, void>;

            template <typename NodeType>
            using CycleGraphGen = basic::CycleGraphGen<NodeType, void>;

            template <typename NodeType>
            using CycleGraph = basic::CycleGraph<NodeType, void>;

            template <typename NodeType>
            using WheelGraphGen = basic::WheelGraphGen<NodeType, void>;

            template <typename NodeType>
            using WheelGraph = basic::WheelGraph<NodeType, void>;

            template <typename NodeType>
            using GridGraphGen = basic::GridGraphGen<NodeType, void>;

            template <typename NodeType>
            using GridGraph = basic::GridGraph<NodeType, void>;

            template <typename NodeType>
            using PseudoTreeGen = basic::PseudoTreeGen<NodeType, void>;

            template <typename NodeType>
            using PseudoTree = basic::PseudoTree<NodeType, void>;

            template <typename NodeType>
            using PseudoInTreeGen = basic::PseudoInTreeGen<NodeType, void>;

            template <typename NodeType>
            using PseudoInTree = basic::PseudoInTree<NodeType, void>;

            template <typename NodeType>
            using PseudoOutTreeGen = basic::PseudoOutTreeGen<NodeType, void>;

            template <typename NodeType>
            using PseudoOutTree = basic::PseudoOutTree<NodeType, void>;

            template <typename NodeType>
            using CactusGen = basic::CactusGen<NodeType, void>;

            template <typename NodeType>
            using Cactus = basic::Cactus<NodeType, void>;

            template <typename NodeType>
            using LinkGen = basic::LinkGen<NodeType, void>;

            template <typename NodeType>
            using Link = basic::Link<NodeType, void>;

            template <typename NodeType>
            using TreeLinkGen = basic::TreeLinkGen<NodeType, void>;

            template <typename NodeType>
            using TreeLink = basic::TreeLink<NodeType, void>;

            template <typename NodeType>
            using ForestGen = basic::ForestGen<NodeType, void>;

            template <typename NodeType>
            using Forest = basic::Forest<NodeType, void>;

            template <typename NodeType>
            using FlowerChainGen = basic::FlowerChainGen<NodeType, void>;

            template <typename NodeType>
            using FlowerChain = basic::FlowerChain<NodeType, void>;
        } // namespace node_weight
    } // namespace rand_graph
} // namespace generator

#endif // !_SGPCET_NODE_WEIGHT_H_
