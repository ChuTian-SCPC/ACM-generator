#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    string s1 = rand_bracket_seq(20);
    string s2 = rand_bracket_seq(10, 20);
    string s3 = rand_bracket_seq(10, 19, "()[]{}");
    string s4 = rand_bracket_seq(10, "%s", "bd");
    cout<<s1<<endl;
    cout<<s2<<endl;
    cout<<s3<<endl;
    cout<<s4<<endl;
    return 0;
}
/*
output:
()()()()(()()(()))()
(((())))()
{}{}{([]())}[{()}]
bdbdbdbdbd
*/