#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    int a = rand_odd(3);
    long long b = rand_odd(3, 1000000000000);
    int c = rand_odd<int>("[%d, %s)", a, "5");
    long long d = rand_odd("[0, %lld]", 30000000000);
    println(a, b ,c, d);
    return 0;
}
/*
output:
1 448254203811 1 29693862033
*/