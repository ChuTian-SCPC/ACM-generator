#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    unweight::HeightTree tree(5);
    tree.gen(3, 4);
    cout<<tree<<endl;
    return 0;
}
/*
output:
5 1
1 5
1 3
3 2
2 4
*/