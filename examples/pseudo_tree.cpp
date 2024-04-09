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
3 5
3 2
1 7
1 6
5 4
2 1
3 7
*/
