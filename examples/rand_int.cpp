#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    int a = rand_int(3);
    long long b = rand_int(3, 1000000000000);
    int c = rand_int("[%d, %s)", a, "5");
    println(a, b ,c);
    return 0;
}
/*
output:
0 724127101906 4
*/