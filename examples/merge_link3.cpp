#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    unweight::Tree tree1(3);
    unweight::Tree tree2(4);
    tree1.set_node_indices({1,2,3});
    tree2.set_node_indices({2,3,4,5});
    tree1.gen();
    tree2.gen();
    
    auto link_graph = unweight::link(tree1, tree2, 2);

    auto link_tree = unweight::link(tree1, tree2);

    cout<<"link to graph:"<<endl;
    cout<<link_graph<<endl;
    cout<<endl;

    cout<<"link to tree:"<<endl;
    cout<<link_tree<<endl;
    return 0;
}
/*
output:
link to graph:
7 7
1 4
1 3
2 3
7 3
5 7
4 6
4 5

link to tree:
7
7 4
5 2
7 5
3 6
1 2
2 3
*/
