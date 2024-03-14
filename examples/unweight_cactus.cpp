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
1 9
7 6
8 4
3 9
3 7
7 2
7 8
6 8
1 3
8 5
5 4
3 2
*/