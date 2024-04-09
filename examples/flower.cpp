#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    unweight::Flower tree(5);
    tree.gen();
    cout<<tree<<endl;
    return 0;
}
/*
output:
5
3 1
4 3
2 3
3 5
*/
