#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    double a = rand_real();
    double b = rand_real(3, 4.0);
    double c = rand_real("[%s, 1e2]","1E-2");
    double d = rand_real("[0.001, 0.001]");
    printf("%lf %.1lf %.2lf\n", a, b, c);
    printf("Correct: %.3lf \n", d, d);
    //Because the precision of d is only up to 10^-3
    //Outputting more digits will result in values outside the range.
    printf("Wrong: %.4lf %lf\n", d, d); 
    return 0;
}
/*
output:
0.306068 3.3 19.30
Correct: 0.001
Wrong: 0.0011 0.001093
*/