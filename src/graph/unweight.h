#ifndef _SGPCET_UNWEIGHT_H_
#define _SGPCET_UNWEIGHT_H_

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
#ifndef _SGPCET_DEGREE_TREE_H_
#include "degree_tree.h"
#endif // !_SGPCET_DEGREE_TREE_H_
#ifndef _SGPCET_SON_TREE_H_
#include "son_tree.h"
#endif // !_SGPCET_SON_TREE_H_
#ifndef _SGPCET_GRAPH_H_
#include "graph.h"
#endif // !_SGPCET_GRAPH_H_
#ifndef _SGPCET_BIPARTITE_GRAPH_H_
#include "bipartite_graph.h"
#endif // !_SGPCET_BIPARTITE_GRAPH_H_
#ifndef _SGPCET_DAG_H_
#include "dag.h"
#endif // !_SGPCET_DAG_H_
#ifndef _SGPCET_CYCLE_GRAPH_H_
#include "cycle_graph.h"
#endif // !_SGPCET_CYCLE_GRAPH_H_
#ifndef _SGPCET_WHEEL_GRAPH_H_
#include "wheel_graph.h"
#endif // !_SGPCET_WHEEL_GRAPH_H_

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
        } // namespace unweight
    } // namespace rand_graph
} // namespace generator

#endif // !_SGPCET_UNWEIGHT_H_
