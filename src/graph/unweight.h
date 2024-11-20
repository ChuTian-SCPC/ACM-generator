#ifndef _SGPCET_UNWEIGHT_H_
#define _SGPCET_UNWEIGHT_H_

#ifndef _SGPCET_TREE_H_
#include "tree.h"
#endif // !_SGPCET_TREE_H_
#ifndef _SGPCET_CHAIN_H_
#include "chain.h"
#endif // !_SGPCET_CHAIN_H_

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
        } // namespace unweight
    } // namespace rand_graph
} // namespace generator

#endif // !_SGPCET_UNWEIGHT_H_
