#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    unweight::Chain tree(5);
    tree.gen();
    cout<<tree<<endl;
    return 0;
}
/*
output:
5
5 1
4 2
4 3
2 1
*/