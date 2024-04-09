#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    unweight::Tree tree1(5, 1);
    tree1.set_tree_generator(unweight::TreeGenerator::Pruefer);
    tree1.gen();
    cout<<"unweight Tree :"<<endl;
    tree1.println();
    edge_weight::Tree<int> tree2(5, 1);
    tree2.use_pruefer();
    tree2.set_edges_weight_function([](){
    	return rand_int(1,100);
	});
    tree2.gen();
    cout<<"edge weight Tree :"<<endl;
    tree2.println();
    return 0;
}
/*
output:
unweight Tree :
5
1 3
1 4
3 5
2 1
edge weight Tree :
5
5 1 30
2 5 71
2 4 6
5 3 57
*/
