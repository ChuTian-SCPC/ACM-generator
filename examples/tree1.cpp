#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

char tree3_edge_random() {
	return rand_char();
}
int main()
{
    init_gen();
    unweight::Tree tree1(5, 1, true, 2);
    tree1.gen();
    cout<<"unweight Tree :"<<endl;
    cout<<tree1<<endl;
    
    node_weight::Tree<int> tree2(5, 1, true, 2, [](){
    	return rand_int(1, 10);
	});
	tree2.gen();
	cout<<"node weight Tree :"<<endl;
	tree2.println();
	
	edge_weight::Tree<char> tree3(5, 1, true, 2);
	tree3.set_edges_weight_function(tree3_edge_random);
	tree3.gen();
	cout<<"edge weigth Tree :"<<endl;
	cout<<tree3<<endl;
	
	both_weight::Tree<double, string> tree4(5, 1, true, 2);
	tree4.set_edges_weight_function([](){
		return rand_string(1, 10, UpperLetter);
	});
	tree4.set_nodes_weight_function([](){
		return rand_real(0.0, 1.0);
	});
	tree4.gen();
	cout<<"both(node and edge) weight Tree :"<<endl;
	tree4.println();
    return 0;
}
/*
output:
unweight Tree :
5 2
2 5
4 3
2 4
4 1
node weight Tree :
5 2
8 9 8 2 7
2 4
3 5
5 1
2 3
edge weigth Tree :
5 2
2 1 u
1 4 w
1 3 t
1 5 d
both(node and edge) weight Tree :
5 2
0.521308 0.857859 0.407227 0.584173 0.674214
5 1 ZSZFWTZFPN
2 5 OTWFLEGS
2 3 T
2 4 GUEMW
*/
