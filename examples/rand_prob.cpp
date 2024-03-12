#include<bits/stdc++.h>
#include"../generator.h"

using namespace std;
using namespace generator::all;

int main()
{
    init_gen();
    map<char, int> m = {
      {'a', 3},
      {'b', 1},
      {'c', 6}
    };
    int cnt[3] = {0, 0, 0};
    for(int i = 1; i <= 10000; i++) {
      cnt[rand_prob(m) - 'a'] ++;
    }
    for(auto [key, value]:m) {
      printf("The probability of randomly generating the Key '%c' is %d/10. After randomizing it 10,000 times, it occurred %d times.\n", key, value, cnt[key - 'a']);
    }
    return 0;
}
/*
output:
The probability of randomly generating the Key 'a' is 3/10. After randomizing it 10,000 times, it occurred 2974 times.
The probability of randomly generating the Key 'b' is 1/10. After randomizing it 10,000 times, it occurred 976 times.
The probability of randomly generating the Key 'c' is 6/10. After randomizing it 10,000 times, it occurred 6050 times.
*/