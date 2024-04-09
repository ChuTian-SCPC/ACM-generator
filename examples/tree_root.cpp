#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    unweight::Tree tree(5, 1, true, 1);
    tree.set_output_root(false);
    cout<<"Tree 1:"<<endl;
    tree.gen(); // success
    cout<<tree<<endl;
    cout<<"Tree 2:"<<endl;
    tree.set_root(6);
    tree.gen(); // fail
    cout<<tree<<endl;
    return 0;
}
/*
output:
Tree 1:
5
1 5
4 2
1 4
4 3
Tree 2:
FAIL restriction of the root is [1, 5], but found 6.
*/
