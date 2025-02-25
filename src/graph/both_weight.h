#ifndef _SGPCET_BOTH_WEIGHT_H_
#define _SGPCET_BOTH_WEIGHT_H_

#ifndef _SGPCET_ALL_TREE_GRAPH_H_
#include "all_tree_graph.h"
#endif // !_SGPCET_ALL_TREE_GRAPH_H_

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

            template <typename NodeType, typename EdgeType>
            using FlowerGen = basic::FlowerGen<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using Flower = basic::Flower<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using HeightTreeGen = basic::HeightTreeGen<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using HeightTree = basic::HeightTree<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using MaxDegreeTreeGen = basic::MaxDegreeTreeGen<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using MaxDegreeTree = basic::MaxDegreeTree<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using MaxSonTreeGen = basic::MaxSonTreeGen<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using MaxSonTree = basic::MaxSonTree<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using GraphGen = basic::GraphGen<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using Graph = basic::Graph<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using BipartiteGraphGen = basic::BipartiteGraphGen<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using BipartiteGraph = basic::BipartiteGraph<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using DAGGen = basic::DAGGen<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using DAG = basic::DAG<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using CycleGraphGen = basic::CycleGraphGen<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using CycleGraph = basic::CycleGraph<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using WheelGraphGen = basic::WheelGraphGen<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using WheelGraph = basic::WheelGraph<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using GridGraphGen = basic::GridGraphGen<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using GridGraph = basic::GridGraph<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using PseudoTreeGen = basic::PseudoTreeGen<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using PseudoTree = basic::PseudoTree<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using PseudoInTreeGen = basic::PseudoInTreeGen<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using PseudoInTree = basic::PseudoInTree<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using PseudoOutTreeGen = basic::PseudoOutTreeGen<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using PseudoOutTree = basic::PseudoOutTree<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using CactusGen = basic::CactusGen<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using Cactus = basic::Cactus<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using LinkGen = basic::LinkGen<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using Link = basic::Link<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using TreeLinkGen = basic::TreeLinkGen<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using TreeLink = basic::TreeLink<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using ForestGen = basic::ForestGen<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using Forest = basic::Forest<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using FlowerChainGen = basic::FlowerChainGen<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using FlowerChain = basic::FlowerChain<NodeType, EdgeType>;
        } // namespace both_weight
    } // namespace rand_graph
} // namespace generator

#endif // !_SGPCET_BOTH_WEIGHT_H_
