#ifndef _SGPCET_EDGE_WEIGHT_H_
#define _SGPCET_EDGE_WEIGHT_H_

#ifndef _SGPCET_ALL_TREE_GRAPH_H_
#include "all_tree_graph.h"
#endif // !_SGPCET_ALL_TREE_GRAPH_H_

namespace generator {
    namespace rand_graph {
        namespace edge_weight {
            template <typename EdgeType>
            using Edge = basic::_Edge<EdgeType>;

            using NodeWeight = basic::_Node<void>;

            using TreeGenerator = _enum::TreeGenerator;

            template <typename EdgeType>
            using TreeGen = basic::TreeGen<void, EdgeType>;

            template <typename EdgeType>
            using RandomFatherGen = basic::RandomFatherGen<void, EdgeType>;

            template <typename EdgeType>
            using PrueferGen = basic::PrueferGen<void, EdgeType>;

            template <typename EdgeType>
            using Tree = basic::Tree<void, EdgeType>;

            template <typename EdgeType>
            using ChainGen = basic::ChainGen<void, EdgeType>;

            template <typename EdgeType>
            using Chain = basic::Chain<void, EdgeType>;

            template <typename EdgeType>
            using FlowerGen = basic::FlowerGen<void, EdgeType>;

            template <typename EdgeType>
            using Flower = basic::Flower<void, EdgeType>;

            template <typename EdgeType>
            using HeightTreeGen = basic::HeightTreeGen<void, EdgeType>;

            template <typename EdgeType>
            using HeightTree = basic::HeightTree<void, EdgeType>;

            template <typename EdgeType>
            using MaxDegreeTreeGen = basic::MaxDegreeTreeGen<void, EdgeType>;

            template <typename EdgeType>
            using MaxDegreeTree = basic::MaxDegreeTree<void, EdgeType>;

            template <typename EdgeType>
            using MaxSonTreeGen = basic::MaxSonTreeGen<void, EdgeType>;

            template <typename EdgeType>
            using MaxSonTree = basic::MaxSonTree<void, EdgeType>;

            template <typename EdgeType>
            using GraphGen = basic::GraphGen<void, EdgeType>;

            template <typename EdgeType>
            using Graph = basic::Graph<void, EdgeType>;

            template <typename EdgeType>
            using BipartiteGraphGen = basic::BipartiteGraphGen<void, EdgeType>;

            template <typename EdgeType>
            using BipartiteGraph = basic::BipartiteGraph<void, EdgeType>;

            template <typename EdgeType>
            using DAGGen = basic::DAGGen<void, EdgeType>;

            template <typename EdgeType>
            using DAG = basic::DAG<void, EdgeType>;

            template <typename EdgeType>
            using CycleGraphGen = basic::CycleGraphGen<void, EdgeType>;

            template <typename EdgeType>
            using CycleGraph = basic::CycleGraph<void, EdgeType>;

            template <typename EdgeType>
            using WheelGraphGen = basic::WheelGraphGen<void, EdgeType>;

            template <typename EdgeType>
            using WheelGraph = basic::WheelGraph<void, EdgeType>;

            template <typename EdgeType>
            using GridGraphGen = basic::GridGraphGen<void, EdgeType>;

            template <typename EdgeType>
            using GridGraph = basic::GridGraph<void, EdgeType>;

            template <typename EdgeType>
            using PseudoTreeGen = basic::PseudoTreeGen<void, EdgeType>;

            template <typename EdgeType>
            using PseudoTree = basic::PseudoTree<void, EdgeType>;

            template <typename EdgeType>
            using PseudoInTreeGen = basic::PseudoInTreeGen<void, EdgeType>;

            template <typename EdgeType>
            using PseudoInTree = basic::PseudoInTree<void, EdgeType>;

            template <typename EdgeType>
            using PseudoOutTreeGen = basic::PseudoOutTreeGen<void, EdgeType>;

            template <typename EdgeType>
            using PseudoOutTree = basic::PseudoOutTree<void, EdgeType>;

            template <typename EdgeType>
            using CactusGen = basic::CactusGen<void, EdgeType>;

            template <typename EdgeType>
            using Cactus = basic::Cactus<void, EdgeType>;

            template <typename EdgeType>
            using LinkGen = basic::LinkGen<void, EdgeType>;

            template <typename EdgeType>
            using Link = basic::Link<void, EdgeType>;

            template <typename EdgeType>
            using TreeLinkGen = basic::TreeLinkGen<void, EdgeType>;

            template <typename EdgeType>
            using TreeLink = basic::TreeLink<void, EdgeType>;

            template <typename EdgeType>
            using ForestGen = basic::ForestGen<void, EdgeType>;

            template <typename EdgeType>
            using Forest = basic::Forest<void, EdgeType>;

            template <typename EdgeType>
            using FlowerChainGen = basic::FlowerChainGen<void, EdgeType>;

            template <typename EdgeType>
            using FlowerChain = basic::FlowerChain<void, EdgeType>;

            template <typename EdgeType>
            using DegreeTree = basic::DegreeTree<void, EdgeType>;

            template <typename EdgeType>
            using DegreeTreeGen = basic::DegreeTreeGen<void, EdgeType>;
        } // namespace edge_weight
    } // namespace rand_graph
} // namespace generator

#endif // !_SGPCET_EDGE_WEIGHT_H_
