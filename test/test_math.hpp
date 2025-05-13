#pragma once
#include "test_basic.hpp"
using namespace generator::all;

std::vector<long long> brute_multiply(const std::vector<long long>& a, const std::vector<long long>& b, long long MOD = 998244353) {
    std::vector<long long> c(a.size() + b.size() - 1, 0);
    for (size_t i = 0; i < a.size(); ++i)
        for (size_t j = 0; j < b.size(); ++j)
            c[i + j] = (c[i + j] + a[i] * b[j]) % MOD;
    return c;
}

bool ntt_multiply_test() {
    long long l = 1000; 
    int n = 10;
    std::vector<long long> a = rand_vector<long long>(1, n, [&](){return rand_int(0, l);});
    std::vector<long long> b = rand_vector<long long>(1, n, [&](){return rand_int(0, l);});
    std::vector<long long> c = NTT<>::multiply(a, b);
    std::vector<long long> d = brute_multiply(a, b);
    for (size_t i = 0; i < d.size(); i++) {
        CHECK(c[i] == d[i]);
    }
    return true;
}

TEST_CASE("test ntt", "[math][ntt]") {
    loop_check([&]() { return ntt_multiply_test();}, 10);
}