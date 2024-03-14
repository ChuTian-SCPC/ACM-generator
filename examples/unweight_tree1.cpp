#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    unweight::Tree tree(5, 1, true, 2);
    tree.gen();
    cout<<tree<<endl;
    return 0;
}
/*
output:
5 2
2 5
4 3
2 4
4 1
*/