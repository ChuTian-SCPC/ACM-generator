#include "test_basic.hpp"
using namespace generator::all;

class TestLinkCheck : public basic::LinkGen<void, void> {
public:
    using Context = basic::Link<void, void>;
    using Super = basic::LinkGen<void, void>;

    TestLinkCheck(Context& context) : Super(context) {}

    std::map<std::pair<int, int>, int> node_merge_map() {
        return _node_merge_map;
    }
};

bool check_node_indices(std::vector<int> expect, std::vector<int> real) {
    println(expect);
    println(real);
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
    auto e1 = g1.edges_ref();
    e1 = {
        {0, 1},
        {1, 2},
        {1, 3},
        {2, 3}
    };
    unweight::Graph g2(3, 2);
    g2.set_node_indices({3, 4, 5});
    auto e2 = g2.edges_ref();
    e2 = {
        {0, 1},
        {1, 2}
    };

    unweight::Link l;
    // target
    l.set_extra_edges_count(0);
    l.set_graph_generator(new TestLinkCheck(l));
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

    auto mp = dynamic_cast<TestLinkCheck*>(l.generator())->node_merge_map();
    exp_edges = {
        {mp[{0, 0}], mp[{0, 1}]},
        {mp[{0, 1}], mp[{0, 2}]},
        {mp[{0, 1}], mp[{0, 3}]},
        {mp[{0, 2}], mp[{0, 3}]},
        {mp[{1, 0}], mp[{1, 1}]},
        {mp[{1, 1}], mp[{1, 2}]}
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