#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    Point<int> p1;
    p1.rand(1, 2, 3, 4);
    Point<int> p2;
    p2.rand("[-1,1]");
    Point<double> p3;
    p3.rand("y[-1, 0) x(0, 1]");
    cout<<p1<<endl;
    cout<<p2<<endl;
    cout<<p3<<endl;
    return 0;
}
/*
output:
2 3
-1 0
0.886319 -0.267002
*/