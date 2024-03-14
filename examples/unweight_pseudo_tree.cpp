#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    unweight::PseudoTree graph(7);
    graph.set_cycle(4);
    graph.gen();
    cout<<graph<<endl;
    return 0;
}
/*
output:
7 7
3 2
4 5
3 7
1 7
1 2
6 2
1 5
*/