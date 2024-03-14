#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    unweight::PseudoInTree graph(7);
    graph.set_cycle(4);
    graph.gen();
    cout<<graph<<endl;
    return 0;
}
/*
output:
7 7
5 3
3 2
1 7
6 1
4 5
2 1
7 3
*/