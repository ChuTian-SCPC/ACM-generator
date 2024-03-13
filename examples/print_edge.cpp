#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    unweight::Edge e(1, 2);
    cout<<e<<endl;
    e.println();
    e.print("%v <- %u");
    return 0;
}
/*
output:
1 2
1 2
2 <- 1
*/