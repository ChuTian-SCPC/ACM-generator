#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    unweight::SonTree tree(10);
    tree.set_output_root(false);
    tree.set_max_son(2);
    tree.gen();
    cout<<tree<<endl;
    return 0;
}
/*
output:
10
1 10
2 8
1 3
8 6
10 2
3 9
3 7
7 4
2 5
*/
