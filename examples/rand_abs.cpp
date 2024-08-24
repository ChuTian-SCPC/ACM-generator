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
    long long h = rand_abs<long long>("[1, %s]", "1e9");
    printf("Type of a is %s, value is %lf\n", ensure_type(a), a);
    printf("Type of b is %s, value is %s\n", ensure_type(b), to_string(b).c_str());
    printf("Type of c is %s, value is %s\n", ensure_type(c), to_string(c).c_str());
    printf("Type of d is %s, value is %s\n", ensure_type(d), to_string(d).c_str());
    printf("Type of e is %s, value is %s\n", ensure_type(e), to_string(e).c_str());
    printf("Type of f is %s, value is %s\n", ensure_type(f), to_string(f).c_str());
    printf("Type of g is %s, value is %.1f\n", ensure_type(g), g);
    printf("Type of h is %s, value is %s\n", ensure_type(h), to_string(h).c_str());
    return 0;
}
/*
output:
Type of a is double, value is -0.306068
Type of b is int, value is -1
Type of c is double, value is -1.712799
Type of d is float, value is -1.735399
Type of e is long long, value is -3
Type of f is double, value is -2.465996
Type of g is double, value is 1.4
Type of h is long long, value is -971407775
*/