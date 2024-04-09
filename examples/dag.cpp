#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    unweight::DAG graph(5, 6);
    graph.set_connect(true);
    graph.gen();
    cout<<graph<<endl;
    return 0;
}
/*
output:
5 6
4 2
2 5
3 1
3 5
3 4
4 1
*/
