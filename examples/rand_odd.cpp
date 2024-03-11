#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    int a = rand_odd(3);
    long long b = rand_odd(3, 1000000000000);
    int c = rand_odd("[%d, %s)", a, "5");
    println(a, b ,c);
    return 0;
}
/*
output:
3 501767942415 3
*/