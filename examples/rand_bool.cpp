#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    bool a = rand_bool();
    cout<<boolalpha<<a<<endl;
    return 0;
}
/*
output:
false
*/