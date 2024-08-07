#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    Point<int> p1 = rand_point<int>(1, 2, 3, 4);
    Point<int> p2 = rand_point<int>("[-1,1]");
    Point<double> p3 = rand_point<double>("y[-1, 0) x(0, 1]");
    Point<int> p4 = rand_point<int>(-1, 1);
    cout<<p1<<endl;
    cout<<p2<<endl;
    cout<<p3<<endl;
    cout<<p4<<endl;
    return 0;
}
/*
output:
2 3
-1 0
0.886319 -0.267002
-1 -1
*/