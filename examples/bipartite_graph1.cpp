#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    unweight::BipartiteGraph graph(5, 6);
    graph.gen();
    cout<<graph<<endl;
    return 0;
}
/*
output:
5 6
4 2
4 1
3 1
3 5
3 2
4 5
*/
