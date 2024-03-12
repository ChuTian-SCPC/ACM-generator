#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    vector<int> a = rand_sum(5, 10);
    vector<int> b = rand_sum(5, 10, 2);
    vector<int> c = rand_sum(5, 10, -8, 5);
    println(a);
    println(b);
    println(c);
    return 0;
}
/*
output:
2 5 1 1 1
2 2 2 2 2
4 5 0 -1 2
*/