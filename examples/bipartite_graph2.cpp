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
3 5
7 6
3 4
2 5
6 2
7 4
7 1
6 3
1 2
7 5
*/
