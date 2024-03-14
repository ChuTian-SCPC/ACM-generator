#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    unweight::Graph graph(5, 6, 0);
    graph.set_connect(true);
    graph.gen();
    cout<<graph<<endl;
    return 0;
}
/*
output:
5 6
2 4
3 1
4 1
3 2
3 4
1 0
*/