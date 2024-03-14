#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    unweight::SonTree tree(10);
    tree.set_output_root(false);
    tree.gen(2);
    cout<<tree<<endl;
    return 0;
}
/*
output:
10
3 4
8 3
1 8
8 6
7 5
2 7
10 2
1 10
3 9
*/