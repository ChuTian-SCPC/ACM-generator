#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    string a = rand_string(4);
    string b = rand_string(3, 5, CharType::ZeroOne);
    string c = rand_string(7, "[string]");
    string d = rand_string(3, 20, "[A%c2]",'d');
    println(a);
    println(b);
    println(c);
    println(d);
    return 0;
}
/*
output:
mzbm
101
ngtttgn
d2ddddd2dAd2AA2dA
*/