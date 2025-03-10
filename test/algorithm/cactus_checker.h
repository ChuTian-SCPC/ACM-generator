#include "../generator.h"
using namespace generator::all;

struct CactusChecker {
    int node_count;
    int edge_count;
    std::vector<std::vector<int>> edges;
    std::vector<int> dfn, low;
    int time;
    bool cactus;

    CactusChecker(int n, int m) : node_count(n), edge_count(m) {
        init();
    }

    void resize(int n, int m) {
        node_count = n;
        edge_count = m;
        init();
    }

    template<typename T>
    void add_edge(basic::_Edge<T> e) {
        int u = e.u();
        int v = e.v();
        edges[u].emplace_back(v);
        edges[v].emplace_back(u);
    }


    void dfs(int u, int fa) {
        time++;
        dfn[u] = time;
        low[u] = time;
        int cnt = 0;
        for (auto& v : edges[u]) {
            if (v == fa) continue;
            if (!dfn[v]) {
                dfs(v, u);
                low[u] = std::min(low[u], low[v]);                
            }
            else low[u] = std::min(low[u], dfn[v]);
            if (low[v] < dfn[u]) { 
                cnt++;
                if (cnt == 2) {
                    cactus = false;
                    return;
                }
            }
            if (!cactus) return;
        }
    }

    bool is_cactus() {
        dfs(0, -1);
        for (auto& d : dfn) if (!d) return false;
        return cactus;
    }
protected:

    void init() {
        edges.clear();
        dfn.clear();
        low.clear();
        edges.resize(node_count);
        dfn.resize(node_count, 0);
        low.resize(edge_count, 0);
        time = 0;
        cactus = true;
    }
};