#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;
 
template<typename T>
const char * ensure_type(T x) {
    if(typeid(x) == typeid(int)) return "int";
    if(typeid(x) == typeid(double))  return "double";
    if(typeid(x) == typeid(float))  return "float";
    if(typeid(x) == typeid(long long))   return "long long";
    return "unknown";
}
int main()
{
    init_gen();
    double a = rand_abs();
    auto b = rand_abs(3);
    auto c = rand_abs(3.0);
    auto d = rand_abs(3.0f);
    auto e = rand_abs(1, 3LL);
    auto f = rand_abs(1.0, 3LL);
    double g = rand_abs("(1e-1, %d]", 2);
    printf("%lf\n", a); 
    printf("Type of b is %s, value is %s\n", ensure_type(b), to_string(b).c_str());
    printf("Type of c is %s, value is %s\n", ensure_type(c), to_string(c).c_str());
    printf("Type of d is %s, value is %s\n", ensure_type(d), to_string(d).c_str());
    printf("Type of e is %s, value is %s\n", ensure_type(e), to_string(e).c_str());
    printf("Type of f is %s, value is %s\n", ensure_type(f), to_string(f).c_str());
    printf("%.1lf\n", g);
    return 0;
}
/*
output:
-0.306068
Type of b is int, value is 1
Type of c is double, value is 0.692376
Type of d is float, value is 1.767953
Type of e is long long, value is -2
Type of f is double, value is -2.531838
0.4
*/