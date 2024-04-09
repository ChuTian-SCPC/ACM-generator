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
1 2
4 3
4 2
3 5
5 1
*/
