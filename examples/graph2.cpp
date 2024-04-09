#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    unweight::Graph graph;
    graph.set_node_count(4);
    graph.set_edge_count(10);
    graph.set_begin_node(1);
    graph.set_multiply_edge(true);
    graph.set_direction(true);
    graph.gen();
    graph.set_output_edge_count(false);
    graph.println();
    return 0;
}
/*
output:
WARN Setting `swap_node` to false, becase `direction` changed! // std::cerr 
4
2 4
3 1
2 4
1 3
1 3
1 3
4 1
4 1
2 1
4 3
*/
