#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    vector<int> a = rand_p(4, 3);
    println(a);
    return 0;
}
/*
output:
5 6 4 3
*/