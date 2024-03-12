#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    vector<int> a = {3, 1, 2};
    vector<int> b = shuffle_index(a);
    int c[5] = {1, 2, 3, 4, 5};
    vector<int> d = shuffle_index(c + 1 ,c + 6, 1);
    println(b);
    println(d);
    return 0;
}
/*
output:
0 0 2 2 0 1
1 3 4 4 4 2 3 3 4 2 2 4 3 1
*/