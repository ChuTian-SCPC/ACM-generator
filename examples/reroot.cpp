#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    unweight::Tree tree(5, 1, true, 1);
    tree.set_output_root(false);
    tree.gen();
    cout<<"root = 1:"<<endl;
    cout<<tree<<endl;
    tree.reroot(2);
    cout<<"root = 2:"<<endl;
    cout<<tree<<endl;
    return 0;
}
/*
output:
root = 1:
5
1 5
4 2
1 4
4 3
root = 2:
5
1 5
4 1
4 3
2 4
*/
