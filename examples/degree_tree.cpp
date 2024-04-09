#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    unweight::DegreeTree tree(10);
    tree.set_max_degree(3);
    tree.gen();
    cout<<tree<<endl;
    return 0;
}
/*
output:
10
10 8
7 2
8 3
8 5
2 10
3 9
6 3
7 1
2 4
*/
