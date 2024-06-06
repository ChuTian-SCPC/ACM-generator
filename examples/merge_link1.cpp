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
    
    unweight::Graph direct_type = unweight::merge(tree1, tree2, unweight::MergeType::Direct);

    unweight::Graph increase_type = unweight::merge(tree1, tree2, unweight::MergeType::Increase);

    unweight::Graph shuffle_type = unweight::merge(tree1, tree2, unweight::MergeType::Shuffle);

    unweight::Graph dedupe_type = unweight::merge(tree1, tree2, unweight::MergeType::Dedupe);

    cout<<"Basic Tree1:"<<endl;
    cout<<tree1<<endl;
    cout<<"Basic Tree2:"<<endl;
    cout<<tree2<<endl;
    cout<<endl;
    
    cout<<"Direct:"<<endl;
    cout<<direct_type<<endl;
    cout<<endl;

    cout<<"Increase:"<<endl;
    cout<<increase_type<<endl;
    cout<<endl;

    cout<<"Shuffle:"<<endl;
    cout<<shuffle_type<<endl;
    cout<<endl;

    cout<<"Dedupe:"<<endl;
    cout<<dedupe_type<<endl;
    return 0;
}
/*
output:
Basic Tree1:
3
2 1
1 3
Basic Tree2:
4
4 5
3 4
2 3

Direct:
7 5
3 4
1 2
3 2
3 1
4 5

Increase:
7 5
5 4
1 3
7 6
5 6
2 1

Shuffle:
7 5
2 7
3 7
5 1
5 4
6 4

Dedupe:
5 5
4 5
3 1
4 3
1 2
3 2
*/
