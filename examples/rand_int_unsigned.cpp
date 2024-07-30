#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    
    long long ll_min = numeric_limits<long long>::min();
    long long ll_max = numeric_limits<long long>::max();
    unsigned int ui_max = numeric_limits<unsigned int>::max();
    unsigned long ul_min = numeric_limits<unsigned long>::min();
    unsigned long ul_max = numeric_limits<unsigned long>::max();
    unsigned long long ull_min = numeric_limits<unsigned long long>::min();
    unsigned long long ull_max = numeric_limits<unsigned long long>::max();
    unsigned long long ull_mid = (unsigned long long)ll_max + 1ULL;

    // these will fail when use testlib
    // long long a = rnd.next(ll_min, ll_max);
    // unsigned int b = rnd.next(ui_max);
    // unsigned long c = rnd.next(ul_min, ul_max);
    // unsigned long long d = rnd.next(ull_min, ull_max);
    // unsigned long long e = rnd.next(ull_mid);
    
    long long a = rand_int(ll_min, ll_max);
    unsigned int b = rand_int(ui_max);
    unsigned long c = rand_int(ul_min, ul_max);
    unsigned long long d = rand_int(ull_min, ull_max);
    unsigned long long e = rand_int(ull_mid);
    cout<<a<<endl;
    cout<<b<<endl;
    cout<<c<<endl;
    cout<<d<<endl;
    cout<<e<<endl;
    return 0;
}
/*
output:
-23238125122187448
760727496
3116585374
5892481728019756187
2654298380427821874
*/