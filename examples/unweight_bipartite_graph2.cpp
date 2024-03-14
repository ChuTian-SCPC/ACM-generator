#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    unweight::BipartiteGraph graph;
    graph.set_left_right(3, 4);
    graph.set_edge_count(10);
    graph.set_connect(true);
    graph.set_swap_node(true);
    graph.gen();
    graph.println();
    return 0;
}
/*
output:
7 10
1 3
6 3
3 5
5 2
7 6
1 7
2 6
7 4
5 7
1 2
*/