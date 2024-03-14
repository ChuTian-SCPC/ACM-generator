#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    unweight::CycleGraph graph(5);
    graph.gen();
    graph.set_output_edge_count(false);
    cout<<graph<<endl;
    return 0;
}
/*
output:
5
4 3
5 3
5 1
2 1
4 2
*/