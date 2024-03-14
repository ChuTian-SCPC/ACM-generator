#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    unweight::GridGraph graph(9, 100);
    graph.gen();
    cout<<graph<<endl;
    return 0;
}
/*
output:
WARN number of edges is large than the maximum possible, use upper edges limit 12.
9 12
1 3
5 6
9 1
8 4
9 7
6 8
5 2
3 7
7 2
8 9
4 1
5 9
*/