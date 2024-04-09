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
1 4
9 5
8 9
5 6
7 2
7 9
3 7
8 6
5 2
8 4
9 1
*/
