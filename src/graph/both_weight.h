#ifndef _SGPCET_BOTH_WEIGHT_H_
#define _SGPCET_BOTH_WEIGHT_H_

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
            using DegreeTreeGen = basic::DegreeTreeGen<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using DegreeTree = basic::DegreeTree<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using SonTreeGen = basic::SonTreeGen<NodeType, EdgeType>;

            template <typename NodeType, typename EdgeType>
            using SonTree = basic::SonTree<NodeType, EdgeType>;

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
        } // namespace both_weight
    } // namespace rand_graph
} // namespace generator

#endif // !_SGPCET_BOTH_WEIGHT_H_
