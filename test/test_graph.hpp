#pragma once
#include "test_basic.hpp"
#include "algorithm/DSU.h"
#include "algorithm/Bipartite.h"
#include "algorithm/TopologicalSort.h"
#include "algorithm/Pseudotree_ug.h"
#include "algorithm/cactus_checker.h"
using namespace generator::all;

TEST_CASE("rand 4 type graph", "[rand_graph][rand_graph-graph][Graph]") {
    init_gen();
    unweight::Graph g1(10, 15);
    g1.gen();
    CHECK(g1.node_count() == 10);
    CHECK(g1.edges().size() == 15);
    
    edge_weight::Graph<int> g2(10, 15);
    g2.set_edges_weight_function(int_weight);
    g2.gen();
    CHECK(g2.node_count() == 10);
    CHECK(g2.edges().size() == 15);
    for (auto& e : g2.edges()) CHECK((e.w() >= 1 && e.w() <= 100));

    node_weight::Graph<int> g3(10, 15);
    g3.set_nodes_weight_function(int_weight);
    g3.gen();
    CHECK(g3.node_count() == 10);
    CHECK(g3.edges().size() == 15);
    for (auto& n : g3.nodes_weight()) CHECK((n.w() >= 1 && n.w() <= 100));

    both_weight::Graph<int, int> g4(10, 15);
    g4.set_edges_weight_function(int_weight);
    g4.set_nodes_weight_function(int_weight);
    g4.gen();
    CHECK(g4.node_count() == 10);
    CHECK(g4.edges().size() == 15);
    for (auto& e : g4.edges()) CHECK((e.w() >= 1 && e.w() <= 100));
    for (auto& n : g4.nodes_weight()) CHECK((n.w() >= 1 && n.w() <= 100));
}

bool check_connect(basic::_GenGraph<void, void>& t) {
    if (!t.connect()) return true;
    int n = t.node_count();
    auto edge = t.edges_ref();
    OY::DSUTable<true> d(n);
    for (auto& e : edge) d.unite_by_ID(e.u(), e.v());
    return d.count() == 1;
}

bool check_self_loop(basic::_GenGraph<void, void>& t) {
    if (t.self_loop()) return true;
    for (auto &e : t.edges_ref()) if (e.u() == e.v()) return false;
    return true;
}

bool check_multiply_edge(basic::_GenGraph<void, void>& t) {
    if (t.multiply_edge()) return true;
    std::set<basic::_Edge<void>> s;
    for (auto &e : t.edges_ref()) {
        if (s.count(e)) return false;
        s.insert(e);
    }
    return true;
}

bool graph_check() {
    int n = rand_int(1, 1000);
    unweight::Graph t(n);
    t.set_connect(rand_bool());
    t.set_self_loop(rand_bool());
    t.set_multiply_edge(rand_bool());
    t.set_direction(rand_bool());
    int l = t.connect() ? n - 1 : 0;
    int r = t.multiply_edge() ? (n == 1 ? 0 : 100000) : std::min(n * (n - 1) / 2, 100000);
    if (t.direction()) r *= 2;
    int m = rand_int(l, r);
    t.set_edge_count(m);
    t.gen();
    if (t.node_count()!= n) return false;
    if (t.edges().size()!= m) return false;
    if (!check_connect(t)) return false;
    if (!check_self_loop(t)) return false;
    if (!check_multiply_edge(t)) return false;
    return true;
}

TEST_CASE("rand graph", "[rand_graph][rand_graph-graph][Graph]") {
    init_gen();
    bool f = loop_check([]() { return graph_check();}, 10);
    CHECK(f);
}

bool bipartite_graph_check() {
    unweight::BipartiteGraph g;
    int n = rand_int(1, 1000);
    g.set_node_count(n);
    g.set_connect(rand_bool());
    int m = rand_int(g.connect() ? n - 1 : 0, (n / 2) * (n - n / 2));
    g.set_edge_count(m);
    g.gen();
    OY::Bipartite::Graph c(n, m);
    for (auto& e : g.edges_ref()) c.add_edge(e.u(), e.v());
    return c.calc().second;
}

TEST_CASE("rand bipartite graph", "[rand_graph][rand_graph-graph][BipartiteGraph]") {
    init_gen();
    bool f = loop_check([]() { return bipartite_graph_check();}, 10);
    CHECK(f);
}

bool dag_check() {
    int n = rand_int(1, 1000);
    unweight::DAG g(n);
    g.set_connect(rand_bool());
    int m = rand_int(g.connect() ? 0 : n - 1, (n / 2) * (n - n / 2));
    g.set_edge_count(m);
    g.gen();
    OY::TOPO::Graph c(n, m);
    for (auto& e : g.edges_ref()) c.add_edge(e.u(), e.v());
    return c.calc().second;
}

TEST_CASE("rand dag", "[rand_graph][rand_graph-graph][DAG]") {
    init_gen();
    bool f = loop_check([]() { return dag_check();}, 10);
    CHECK(f);
}

std::vector<int> degree(basic::_GenGraph<void, void>& g) {
    std::vector<int> d(g.node_count(), 0);
    for (auto& e : g.edges_ref()) {
        d[e.u()]++;
        d[e.v()]++;
    }
    return d;
}

bool cycle_graph_check() {
    int n = rand_int(3, 1000);
    unweight::CycleGraph g(n);
    g.gen();
    auto d = degree(g);
    for (auto& deg : d) if (deg != 2) return false;
    return true;
}

TEST_CASE("rand cycle graph", "[rand_graph][rand_graph-graph][CycleGraph]") {
    init_gen();
    bool f = loop_check([]() { return cycle_graph_check();}, 10);
    CHECK(f);
}

bool wheel_graph_check() {
    int n = rand_int(4, 1000);
    unweight::WheelGraph g(n);
    g.gen();
    auto d = degree(g);
    int p = -1;
    for (int i = 0; i < n; i++) {
        if (d[i] == n - 1) {
            if (p != -1) return false;
            p = i;
        }
        else if (d[i] != 3) return false;
    }
    if (p == -1) return false;
    return true; 
}

TEST_CASE("rand wheel graph", "[rand_graph][rand_graph-graph][WheelGraph]") {
    init_gen();
    bool f = loop_check([]() { return wheel_graph_check();}, 10);
    CHECK(f);
}

bool pseudo_tree_check() {
    int n = rand_int(3, 1000);
    unweight::PseudoTree g(n);
    g.gen();
    OY::PsuedoUG::Graph<bool> c(n);
    for (auto& e : g.edges_ref()) c.add_edge(e.u(), e.v());
    c.prepare();
    int s = c.cycle_count();
    if (s != 1) return false;
    return c.get_cycle_info(0).size() == g.cycle();
}

TEST_CASE("rand pseudo_tree","[rand_graph][rand_graph-graph][PseudoTree][PseudoInTree][PseudoOutTree]") {
    // 基环树和内向外向的区别仅在于方向性
    init_gen();
    bool f = loop_check([]() { return pseudo_tree_check();}, 10);
    CHECK(f);
}

bool cactus_check() {
    int n = rand_int(3, 10);
    int m = rand_int(n - 1, n - 1 + (n - 1) / 2);
    unweight::Cactus g(n, m);
    g.gen();
    CactusChecker c(n, m);
    for (auto& e : g.edges_ref()) c.add_edge(e);
    return c.is_cactus();
}

TEST_CASE("rand cactus", "[rand_graph][rand_graph-graph][Cactus]") {
    init_gen();
    bool f = loop_check([]() { return cactus_check();}, 10);
    CHECK(f);
}

class LinkCheck : public basic::LinkGen<void, void> {
public:
    using Context = basic::Link<void, void>;
    using Super = basic::LinkGen<void, void>;

    LinkCheck(Context& context) : Super(context) {}

    std::map<std::pair<int, int>, int> node_merge_map() {
        return _node_merge_map;
    }
};

bool check_node_indices(std::vector<int> expect, std::vector<int> real) {
    int n = expect.size();
    if (n != real.size()) return false;
    for(int i = 0; i < n; i++) if (expect[i] != real[i]) return false;
    return true;
}

bool check_edges(std::vector<basic::_Edge<void>> expect, std::vector<basic::_Edge<void>> real) {
    int n = expect.size();
    if (n != real.size()) return false;
    std::sort(expect.begin(), expect.end());
    std::sort(real.begin(), real.end());
    for (int i = 0; i < n; i++) if (expect[i] != real[i]) return false;
    return true;
}

TEST_CASE("link test", "[rand_graph][Link][TreeLink]") {

    unweight::Graph g1(4, 4);
    g1.set_node_indices({1, 2, 3, 4});
    auto& e1 = g1.edges_ref();
    e1 = {
        {0, 1},
        {1, 2},
        {1, 3},
        {2, 3}
    };
    unweight::Graph g2(3, 2);
    g2.set_node_indices({3, 4, 5});
    auto& e2 = g2.edges_ref();
    e2 = {
        {0, 1},
        {1, 2}
    };

    unweight::Link l;
    // target
    l.set_extra_edges_count(0);
    l.set_graph_generator(new LinkCheck(l));
    l.set_swap_node(false);
    // source
    l.add_source(g1);
    l.add_source(g2);

    l.set_link_type(LinkType::Direct);
    l.gen();
    std::vector<int> exp_node_idx = {1, 2, 3, 4, 3, 4, 5};
    CHECK(check_node_indices(exp_node_idx, l.node_indices()));
    std::vector<basic::_Edge<void>> exp_edges = {
        {1, 2},
        {2, 3},
        {3, 4},
        {2, 4},
        {3, 4},
        {4, 5}
    };
    CHECK(check_edges(exp_edges, l.edges()));

    l.set_link_type(LinkType::Increase);
    l.gen();
    exp_node_idx = {1, 2, 3, 4, 5, 6, 7};
    exp_edges = {
        {1, 2},
        {2, 3},
        {3, 4},
        {2, 4},
        {5, 6},
        {6, 7}
    };
    CHECK(check_node_indices(exp_node_idx, l.node_indices()));
    CHECK(check_edges(exp_edges, l.edges()));

    l.set_link_type(LinkType::Shuffle);
    l.gen();
    auto mp = dynamic_cast<LinkCheck*>(l.generator())->node_merge_map();
    exp_edges = {
        {mp[{0, 0}] + 1, mp[{0, 1}] + 1},
        {mp[{0, 1}] + 1, mp[{0, 2}] + 1},
        {mp[{0, 1}] + 1, mp[{0, 3}] + 1},
        {mp[{0, 2}] + 1, mp[{0, 3}] + 1},
        {mp[{1, 0}] + 1, mp[{1, 1}] + 1},
        {mp[{1, 1}] + 1, mp[{1, 2}] + 1}
    };
    CHECK(check_node_indices(exp_node_idx, l.node_indices()));
    CHECK(check_edges(exp_edges, l.edges()));

    l.set_link_type(LinkType::Dedupe);
    l.gen();
    exp_node_idx = {1, 2, 3, 4, 5};
    exp_edges = {
        {1, 2},
        {2, 3},
        {3, 4},
        {2, 4},
        {4, 5}
    };
    CHECK(check_node_indices(exp_node_idx, l.node_indices()));
    CHECK(check_edges(exp_edges, l.edges()));
}