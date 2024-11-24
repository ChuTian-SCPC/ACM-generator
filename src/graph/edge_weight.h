#ifndef _SGPCET_EDGE_WEIGHT_H_
#define _SGPCET_EDGE_WEIGHT_H_

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
#ifndef _SGPCET_GRID_GRAPH_H_
#include "grid_graph.h"
#endif // !_SGPCET_GRID_GRAPH_H_

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
            using DegreeTreeGen = basic::DegreeTreeGen<void, EdgeType>;

            template <typename EdgeType>
            using DegreeTree = basic::DegreeTree<void, EdgeType>;

            template <typename EdgeType>
            using SonTreeGen = basic::SonTreeGen<void, EdgeType>;

            template <typename EdgeType>
            using SonTree = basic::SonTree<void, EdgeType>;

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
        } // namespace edge_weight
    } // namespace rand_graph
} // namespace generator

#endif // !_SGPCET_EDGE_WEIGHT_H_
