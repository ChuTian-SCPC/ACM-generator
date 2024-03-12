#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

double gen_double() {
  double r = rand_real("[0e-6, %d]", 1);
  return r;
}
int main()
{
    init_gen();
    vector<int> a = rand_vector<int>(5, []() {
      return rand_int(1, 3);
    });
    
    vector<double> b = rand_vector<double>(5, 10, gen_double);
    println(a);
    println(b);
    
    vector<vector<int>> c = rand_vector<vector<int>>(5, []() {
      int len = rand_int(1, 10);
      return rand_vector<int>(len, [&]() {
        return rand_int(1, len);
      });
    });
    printf("Matrix C:\n");
    for(auto x : c) {
      println(x);
    }
    return 0;
}
/*
output:
2 2 3 1 1
0.786319 0.732998 0.0997299 0.822404 0.598788 0.449732 0.768084
Matrix C:
1 4 4 2
1 1 3 6 3 7 2 4
4 8 4 7 1 1 7 6
3 1 5 2 3
4 4 3 1
*/