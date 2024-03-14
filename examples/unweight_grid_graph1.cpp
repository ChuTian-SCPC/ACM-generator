#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    unweight::GridGraph graph(9, 10, 1, 2);
    graph.set_connect(true);
    graph.gen();
    cout<<graph<<endl;
    return 0;
}
/*
output:
9 10
8 2
3 5
8 4
7 3
5 4
9 1
6 1
2 7
2 1
8 6
*/