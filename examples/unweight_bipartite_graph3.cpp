#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    unweight::BipartiteGraph graph(7, 10, 0, 3);
    graph.set_different_part(true);
    graph.gen();
    graph.use_type_left_right();
    cout<<graph<<endl;
    return 0;
}
/*
output:
3 4 10
1 0
2 0
2 3
0 1
0 0
0 2
2 2
1 2
1 1
2 1
*/