#pragma once
#include "test_basic.hpp"
using namespace generator::all;

TEST_CASE("numeric format", "[rand_numeric][format]") {
    init_gen();
    // format int range
    auto r1 = __format_to_int_range<int>("[10, 20]");
    CHECK((r1.first == 10 && r1.second == 20));
    auto r2 = __format_to_int_range<int>("[1, 1e9]");
    CHECK((r2.first == 1 && r2.second == 1000000000));
    CHECK(std::is_same<decltype(r2), std::pair<int, int>>::value);
    auto r3 = __format_to_int_range<long long>("[1, 1e9]");
    CHECK(std::is_same<decltype(r3), std::pair<long long, long long>>::value);
    auto r4 = __format_to_int_range<int>("[1, 3.3)");
    CHECK((r4.first == 1 && r4.second == 2));

    // format double range
    using Catch::Matchers::WithinAbs;
    auto r5 = __format_to_double_range<double>("[1.123,2.456)");
    CHECK_THAT(r5.first, WithinAbs(1.123, 0.0001));
    CHECK_THAT(r5.second, WithinAbs(2.456, 0.0001));
    auto r6 = __format_to_double_range<double>("(1.123,2.456]");
    CHECK_THAT(r6.first, WithinAbs(1.124, 0.0001));
    CHECK_THAT(r6.second, WithinAbs(2.4561, 0.00001));
    auto r7 = __format_to_double_range<double>("[1e-3, 2.34E-2)");
    CHECK_THAT(r7.first, WithinAbs(0.001, 0.0001));
    CHECK_THAT(r7.second, WithinAbs(0.0234, 0.00001));
}

TEST_CASE("rand int testlib fail", "[rand_numeric][rand_int]") {
    init_gen();
    
    long long ll_min = std::numeric_limits<long long>::min();
    long long ll_max = std::numeric_limits<long long>::max();
    unsigned int ui_max = std::numeric_limits<unsigned int>::max();
    unsigned long ul_min = std::numeric_limits<unsigned long>::min();
    unsigned long ul_max = std::numeric_limits<unsigned long>::max();
    unsigned long long ull_min = std::numeric_limits<unsigned long long>::min();
    unsigned long long ull_max = std::numeric_limits<unsigned long long>::max();
    unsigned long long ull_mid = (unsigned long long)ll_max + 1ULL;

    bool f1 = loop_check([&]() {
        long long x = rand_int(ll_min, ll_max);
        return x >= ll_min && x <= ll_max;
    }, 100);
    CHECK(f1);

    bool f2 = loop_check([&]() {
        unsigned int x = rand_int(ui_max);
        return x >= 0 && x < ui_max; 
    }, 100);
    CHECK(f2);

    bool f3 = loop_check([&]() {
        unsigned long x = rand_int(ul_min, ul_max);
        return x >= ul_min && x <= ul_max;
    }, 100);
    CHECK(f3);
    bool f4 = loop_check([&]() {
        unsigned long long x = rand_int(ull_min, ull_max);
        return x >= ull_min && x <= ull_max;
    }, 100);
    CHECK(f4);

    bool f5 = loop_check([&]() {
        unsigned long long x = rand_int(ull_min, ull_mid);
        return x >= 0 && x < ull_mid;
    }, 100);
    CHECK(f5);
}

TEST_CASE("rand odd and even", "[rand_numeric][rand_odd][rand_even]") {
    init_gen();
    _defl.set_log_same(false); //  循环转换范围的log不再重复打印
    bool f1 = loop_check([]() { return rand_odd(3) == 1; }, 100); // 随机[0,3)的奇数，肯定只有1
    CHECK(f1);
    bool f2 = loop_check([]() { return rand_even(2) == 0;}, 100); // 随机[0,2)的偶数，肯定只有0
    CHECK(f2);
    bool f3 = loop_check([]() { return rand_odd(-1, 0) == -1;}, 100); // 随机[-1,0]的奇数，肯定只有-1
    CHECK(f3);
    bool f4 = loop_check([]() { return rand_even(-1, 0) == 0;}, 100); // 随机[-1,0]的偶数，肯定只有0
    CHECK(f4);
    bool f5 = loop_check([]() { return rand_odd("[-1e9, 1e9]") % 2 != 0;}, 100);
    CHECK(f5);
    bool f6 = loop_check([]() { return rand_even("[-1e9, 1e9]") % 2 == 0;}, 100);
    CHECK(f6);
}

TEST_CASE("rand by probility", "[rand_numeric][rand_prob]") {
    init_gen();
    std::map<char, int> m;
    int sum = 0;
    for (int i = 'a'; i <='z'; i++) {
        int p = rand_int(1, 100);
        m[i] = p;
        sum += p;
    }
    std::vector<int> v;
    v.resize(26);

    int rand_time = 1000000;
    for (int i = 0; i < rand_time; i++) {
        char c = rand_prob(m);
        v[c - 'a'] ++;
    }

    for (int i = 'a'; i <= 'z'; i++) {
        double p = (double)v[i - 'a'] / rand_time;
        double q = (double)m[i] / sum;
        CHECK(doubleCompare(p, q, 0.01)); // 1%的误差
    }
}

TEST_CASE("rand range", "[rand_numeric][rand_range]") {
    init_gen();
    bool f1 = loop_check([]() {
        auto x = rand_range(1, 100);
        return x.first >= 1 && x.first <= 100 && x.second >= 1 && x.second <= 100 && x.first <= x.second;
    }, 100);
    CHECK(f1);
    
    auto x1 = rand_range(1, 1);
    CHECK((x1.first == 1 && x1.second == 1));

    bool f2 = loop_check([]() {
        auto x = rand_range(1, 1e9);
        return x.first >= 1 && x.first <= (int)1e9 && x.second >= 1 && x.second <= (int)1e9 && x.first <= x.second;
    }, 100);
    CHECK(f2);
}

TEST_CASE("range contain", "[rand_numeric][IsRangeContained]") {
    init_gen();
    CHECK((IsRangeContained<int, int>::value == true));
    CHECK((IsRangeContained<long long, int>::value == false));
    CHECK((IsRangeContained<int, long long>::value == true));
    CHECK((IsRangeContained<unsigned int, int>::value == false));
    CHECK((IsRangeContained<int, unsigned int>::value == false));
    CHECK((IsRangeContained<unsigned int, long long>::value == true));
    CHECK((IsRangeContained<unsigned int, unsigned long long>::value == true));
    CHECK((IsRangeContained<unsigned long long, unsigned int>::value == false));
    CHECK((IsRangeContained<unsigned long long, long long>::value == false));
    CHECK((IsRangeContained<long long, unsigned long long>::value == false));
    CHECK((IsRangeContained<int, unsigned long long>::value == false));
    CHECK((IsRangeContained<double, int>::value == false));
}