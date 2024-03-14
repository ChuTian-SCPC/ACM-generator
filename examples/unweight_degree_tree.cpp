#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    unweight::DegreeTree tree(10);
    tree.gen(3);
    cout<<tree<<endl;
    return 0;
}
/*
output:
10
2 6
3 8
4 7
2 10
9 3
8 5
3 1
7 10
2 8
*/