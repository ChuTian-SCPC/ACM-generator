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
9 1
2 7
3 7
4 5
2 8
5 3
4 8
6 8
4 7
2 1
*/
