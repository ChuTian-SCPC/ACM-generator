#include "test_basic.hpp"
#include "algorithm/DSU.h"
#include "algorithm/Prufer.h"
#include "algorithm/VectorTree.h"
using namespace generator::all;

template<typename NodeType, typename EdgeType>
bool is_tree(basic::_GenTree<NodeType, EdgeType>& t) {
    int n = t.node_count();
    auto edge = t.edges_ref();
    OY::DSUTable<true> d(n);
    for (auto& e : edge) d.unite_by_ID(e.u(), e.v());
    return d.count() == 1;
}

TEST_CASE("rand 4 type tree", "[rand_graph][rand_graph-tree][Tree]") {
    init_gen();
    unweight::Tree t1(10);
    t1.gen();
    CHECK(t1.node_count() == 10);
    CHECK(t1.edges().size() == 9);
    CHECK(is_tree(t1));

    edge_weight::Tree<int> t2(10);
    t2.set_edges_weight_function(int_weight);
    t2.gen();
    for (auto& e : t2.edges()) CHECK((e.w() >= 1 && e.w() <= 100));
    CHECK(is_tree(t2));

    node_weight::Tree<int> t3(10);
    t3.set_nodes_weight_function(int_weight);
    t3.gen();
    for (auto& n : t3.nodes_weight()) CHECK((n.w() >= 1 && n.w() <= 100));
    CHECK(is_tree(t3)); 

    both_weight::Tree<int, int> t4(10);
    t4.set_edges_weight_function(int_weight);
    t4.set_nodes_weight_function(int_weight);
    t4.gen();
    for (auto& e : t4.edges()) CHECK((e.w() >= 1 && e.w() <= 100));
    for (auto& n : t4.nodes_weight()) CHECK((n.w() >= 1 && n.w() <= 100));
    CHECK(is_tree(t4));
}

TEST_CASE("rand tree root", "[rand_graph][rand_graph-tree][Tree]") {
    init_gen();
    unweight::Tree t(10, 2);
    auto indices = t.node_indices();
    CHECK(indices.size() == 10);
    for (int i = 0; i < indices.size(); i++) {
        CHECK(indices[i] == i + t.begin_node());
    }
    t.set_node_indices(3, -5); // 第三个结点编号为-5
    CHECK(t.node_indices()[2] == -5);
    CHECK(t.is_rooted() == false);
    t.set_is_rooted(true);
    t.set_root(1); // 第一个结点为根结点
    CHECK(t.root() == 2); 
    t.set_root(3); // 第三个结点为根结点
    CHECK(t.root() == -5); // root返回的是结点编号
    CHECK(t.root_ref() == 2); // root_ref返回的是结点下标，从0开始 
}

class PrueferCodeCheck : public basic::BasicPrueferGen<basic::Tree, void, void> {
public:
    using Context = basic::Tree<void, void>;
    using Super = basic::BasicPrueferGen<basic::Tree, void, void>;

    std::vector<int> pruefer;

    PrueferCodeCheck(Context& context) : Super(context) {}

protected:
    virtual void __generate_pruefer() override {
        int node_count = this->_context.node_count();
        std::vector<int> times = rand_sum(node_count, node_count - 2, 0);
        pruefer = shuffle_index(times);
        __pruefer_decode(pruefer);
    }
};

bool pruefer_check() {
    int n = rand_int(3, 1000);
    unweight::Tree t(n);
    t.set_tree_generator(new PrueferCodeCheck(t));
    t.gen();
    auto g_code = dynamic_cast<PrueferCodeCheck*>(t.generator())->pruefer;
    OY::PRUFER::Tree p(n);
    for (auto& e : t.edges_ref()) p.add_edge(e.u(), e.v());
    auto p_code = p.encode();
    for (int i = 0; i < n - 2; i++) 
        if(g_code[i] != p_code[i]) return false;
    return true;
}

TEST_CASE("pruefer rand tree", "[rand_graph][rand_graph-tree][pruefer][Tree]") {
    init_gen();
    bool f = loop_check([]() { return pruefer_check();}, 100);
    CHECK(f);
}

int tree_depth(basic::_GenTree<void, void>& t) {
    int n = t.node_count();
    auto edge = t.edges_ref();
    OY::VectorTree::Tree<bool> T(n);
    for (auto& e : edge) T.add_edge(e.u(), e.v());
    T.prepare();
    int r = t.root_ref();
    T.set_root(r);
    std::vector<int> dep(n, 0);
    T.tree_dp_vertex(r, [&](int a, int p) { dep[a] = (p != -1 ? dep[p] + 1 : 0); }, {}, {});
    int depth = 0;
    for (auto& d : dep) depth = std::max(depth, d);
    return depth + 1;
}

bool is_chain(basic::Chain<void, void>& t) {
    return tree_depth(t) == t.node_count();
}

bool chain_check() {
    int n = rand_int(1, 1000);
    unweight::Chain t(n);
    t.set_is_rooted(true);
    t.set_root(rand_int(1, n));
    t.gen();
    if (t.node_count() != n) return false;
    if (t.edges().size() != n - 1) return false;
    if (!is_tree(t)) return false;
    if (!is_chain(t)) return false;
    return true;
}

TEST_CASE("rand chain", "[rand_graph][rand_graph-tree][Chain]") {
    init_gen();
    bool f = loop_check([]() { return chain_check();}, 10);
    CHECK(f);
}

bool is_flower(basic::Flower<void, void>& t) {
    return tree_depth(t) == std::min(t.node_count(), 2);
}

bool flower_check() {
    int n = rand_int(1, 1000);
    unweight::Flower t(n);
    t.set_is_rooted(true);
    t.set_root(rand_int(1, n));
    t.gen();
    if (t.node_count() != n) return false;
    if (t.edges().size() != n - 1) return false;
    if (!is_tree(t)) return false;
    if (!is_flower(t)) return false;
    return true;
}

TEST_CASE("rand flower", "[rand_graph][rand_graph-tree][Flower]") {
    init_gen();
    bool f = loop_check([]() { return flower_check();}, 10);
    CHECK(f); 
}

bool height_tree_check(int n) {
    unweight::HeightTree t(n);
    t.set_root(rand_int(1, n));
    t.gen();
    if (t.node_count() != n) return false;
    if (t.edges().size() != n - 1) return false;
    if (!is_tree(t)) return false;
    if (tree_depth(t) != t.height()) return false;
    return true;
}

TEST_CASE("rand height tree", "[rand_graph][rand_graph-tree][HeightTree]") {
    init_gen();
    CHECK(height_tree_check(1)); // 特判node_count = 1
    bool f = loop_check([]() { int n = rand_int(2, 1000); return height_tree_check(n);}, 10);
    CHECK(f); 
}

bool max_degree_tree_check(int n) {
    unweight::MaxDegreeTree t(n);
    t.gen();
    auto edge = t.edges_ref();
    OY::VectorTree::Tree<bool> T(n);
    for (auto& e : edge) T.add_edge(e.u(), e.v());
    std::vector<int> deg(n, 0);
    for (int i = 0; i < n; i++) {
        T.do_for_each_adj_vertex(i, [&](int to) { deg[i]++; });
    }
    int max_degree = 0;
    for (auto& d : deg) max_degree = std::max(max_degree, d);
    return max_degree <= t.max_degree();
}

TEST_CASE("rand max degree tree", "[rand_graph][rand_graph-tree][MaxDegreeTree]") {
    init_gen();
    CHECK(max_degree_tree_check(1)); // 特判node_count = 1
    CHECK(max_degree_tree_check(2)); // 特判node_count = 2
    bool f = loop_check([]() { int n = rand_int(3, 1000); return max_degree_tree_check(n);}, 10);
    CHECK(f);
}

bool max_son_tree_check(int n) {
    unweight::MaxSonTree t(n);
    t.gen();
    auto edge = t.edges_ref();
    OY::VectorTree::Tree<bool> T(n);
    for (auto& e : edge) T.add_edge(e.u(), e.v());
    std::vector<int> son(n, 0);
    int r = t.root_ref();
    T.tree_dp_vertex(r, [&](int a, int p) { if (p != -1) son[p]++; }, {}, {});
    int max_son = 0;
    for (auto& s : son) max_son = std::max(max_son, s);
    return max_son <= t.max_son();
}

TEST_CASE("rand max son tree", "[rand_graph][rand_graph-tree][MaxSonTree]") {
    init_gen();
    CHECK(max_son_tree_check(1)); // 特判node_count = 1
    bool f = loop_check([]() { int n = rand_int(2, 10); return max_son_tree_check(n);}, 10);
    CHECK(f);
}

