#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    unweight::WheelGraph graph(5);
    graph.gen();
    graph.set_output_edge_count(false);
    cout<<graph<<endl;
    return 0;
}
/*
output:
5
2 1
3 5
2 5
4 2
4 3
5 4
1 5
3 1
*/