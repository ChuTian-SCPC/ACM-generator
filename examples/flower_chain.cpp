#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    unweight::FlowerChain tree(10);
    tree.set_chain_size(4);
    tree.gen();
    cout<<tree<<endl;
    return 0;
}
/*
output:
10
2 10
3 2
9 8
7 2
3 8
6 1
1 9
2 4
2 5
*/
