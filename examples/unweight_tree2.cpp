#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    unweight::Tree tree(5, 1, true);
    tree.set_root(4);
    tree.set_begin_node(0);
    tree.set_swap_node(true);
    tree.gen();
    tree.set_output_root(false);
    tree.println();
    return 0;
}
/*
output:
5
4 3
1 3
4 2
1 0
*/