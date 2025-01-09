#ifndef _SGPCET_UNWEIGHT_H_
#define _SGPCET_UNWEIGHT_H_

#ifndef _SGPCET_ALL_TREE_GRAPH_H_
#include "all_tree_graph.h"
#endif // !_SGPCET_ALL_TREE_GRAPH_H_

namespace generator {
    namespace rand_graph {
        namespace unweight {
            using Edge = basic::_Edge<void>;
            using NodeWeight = basic::_Node<void>;
            using TreeGenerator = _enum::TreeGenerator;
            using TreeGen = basic::TreeGen<void, void>;
            using RandomFatherGen = basic::RandomFatherGen<void, void>;
            using PrueferGen = basic::PrueferGen<void, void>;
            using Tree = basic::Tree<void, void>;
            using ChainGen = basic::ChainGen<void, void>;
            using Chain = basic::Chain<void, void>;
            using FlowerGen = basic::FlowerGen<void, void>;
            using Flower = basic::Flower<void, void>;
            using HeightTreeGen = basic::HeightTreeGen<void, void>;
            using HeightTree = basic::HeightTree<void, void>;
            using DegreeTreeGen = basic::DegreeTreeGen<void, void>;
            using DegreeTree = basic::DegreeTree<void, void>;
            using SonTreeGen = basic::SonTreeGen<void, void>;
            using SonTree = basic::SonTree<void, void>;
            using GraphGen = basic::GraphGen<void, void>;
            using Graph = basic::Graph<void, void>;
            using BipartiteGraphGen = basic::BipartiteGraphGen<void, void>;
            using BipartiteGraph = basic::BipartiteGraph<void, void>;
            using DAGGen = basic::DAGGen<void, void>;
            using DAG = basic::DAG<void, void>;
            using CycleGraphGen = basic::CycleGraphGen<void, void>;
            using CycleGraph = basic::CycleGraph<void, void>;
            using WheelGraphGen = basic::WheelGraphGen<void, void>;
            using WheelGraph = basic::WheelGraph<void, void>;
            using GridGraphGen = basic::GridGraphGen<void, void>;
            using GridGraph = basic::GridGraph<void, void>;
            using PseudoTreeGen = basic::PseudoTreeGen<void, void>;
            using PseudoTree = basic::PseudoTree<void, void>;
            using PseudoInTreeGen = basic::PseudoInTreeGen<void, void>;
            using PseudoInTree = basic::PseudoInTree<void, void>;
            using PseudoOutTreeGen = basic::PseudoOutTreeGen<void, void>;
            using PseudoOutTree = basic::PseudoOutTree<void, void>;
            using CactusGen = basic::CactusGen<void, void>;
            using Cactus = basic::Cactus<void, void>;
            using LinkGen = basic::LinkGen<void, void>;
            using Link = basic::Link<void, void>;
            using TreeLinkGen = basic::TreeLinkGen<void, void>;
            using TreeLink = basic::TreeLink<void, void>;
        } // namespace unweight
    } // namespace rand_graph
} // namespace generator

#endif // !_SGPCET_UNWEIGHT_H_
