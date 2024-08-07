#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    ConvexHull<int> c(10);
    c.set_xy_limit("[-20, 20]");
    c.gen();
    cout<<c<<endl;
    return 0;
}
/*
output:
10    
13 -13
17 -7 
18 2  
16 15
12 18
4 19
-8 16
-11 10
-13 -13
-2 -18
*/