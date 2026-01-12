#include "../generator.h"
using namespace generator::all;

struct StartReachableGraphChecker{
    std::vector<std::vector<int>> edges;
    int start;
    std::vector<int> visit;

    StartReachableGraphChecker(unweight::StartReachableGraph& g) {
        int n = g.node_count();
        edges.resize(n);
        visit.resize(n, 0);
        auto e = g.edges();
        for (auto& i : e) edges[i.u()].push_back(i.v());
        start = g.start();
    }

    void bfs(int s) {
        std::queue<int> q;
        q.push(s);
        visit[s] = 1;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto& v : edges[u]) {
                if (visit[v] == 0) {
                    visit[v] = 1;
                    q.push(v); 
                } 
            } 
        } 
    }

    bool check() {
        bfs(start);
        for (auto &i : visit) if (i == 0) return false;
        return true;
    }
};