#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    char a = rand_char();
    char b = rand_char(CharType::Number);
    char c = rand_char("[a%cf]",'d');
    println(a, b ,c);
    return 0;
}
/*
output:
m 9 a
*/