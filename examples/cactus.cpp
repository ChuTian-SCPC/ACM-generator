#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    unweight::Cactus graph(9, 12);
    graph.gen();
    cout<<graph<<endl;
    return 0;
}
/*
output:
9 12
8 4
4 5
8 1
6 8
7 6
7 2
8 5
9 8
3 2
7 3
8 7
9 1
*/
