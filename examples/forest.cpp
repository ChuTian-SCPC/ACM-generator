#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    unweight::Forest graph(10, 7);
    graph.gen();
    cout<<graph<<endl;
    return 0;
}
/*
output:
10 7
1 6 
8 2 
9 5 
1 4 
3 10
3 7 
1 3
*/
