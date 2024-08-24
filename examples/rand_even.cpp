#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    int a = rand_even(3);
    long long b = rand_even(3, 1000000000000);
    int c = rand_even<int>("[%d, %s)", a, "5");
    long long d = rand_even("[0, %lld]", 30000000000);
    println(a, b ,c, d);
    return 0;
}
/*
output:
0 448254203812 0 27406335756
*/