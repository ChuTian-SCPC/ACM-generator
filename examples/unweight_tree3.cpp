#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    unweight::Tree tree(5, 1);
    tree.set_tree_generator(unweight::Tree::TreeGenerator::Pruefer);
    tree.gen();
    tree.println();
    return 0;
}
/*
output:
5
3 1
1 2
1 5
3 4
*/