#pragma once
#include "test_basic.hpp"
using namespace generator::all;

TEST_CASE("test big number setting", "[math][big_number][setting]") {
    // 默认情况
    auto l = BigNumberSetting::labels();
    CHECK(l.size() == 36);
    for(int i = 0; i < l.size(); i++) {
        if (i < 10) CHECK(l[i] == std::to_string(i));
        else CHECK(l[i] == std::string(1, 'A' + i - 10));
    }
    // 小写字母
    BigNumberSetting::lower_labels();
    l = BigNumberSetting::labels();
    CHECK(l.size() == 36);
    for(int i = 0; i < l.size(); i++) {
        if (i < 10) CHECK(l[i] == std::to_string(i));
        else CHECK(l[i] == std::string(1, 'a' + i - 10));
    }
    // 自行定义单独
    BigNumberSetting::set_labels(10, 'A');
    BigNumberSetting::set_labels(11, 'B');
    BigNumberSetting::set_labels(12, 'C');
    BigNumberSetting::set_labels(13, 'D');
    BigNumberSetting::set_labels(14, 'E');
    BigNumberSetting::set_labels(15, 'F');
    l = BigNumberSetting::labels();
    for(int i = 0; i < 10; i++) {
        if (i < 10) CHECK(l[i] == std::to_string(i));
        else if (i < 16) CHECK(l[i] == std::string(1, 'A' + i - 10));
        else CHECK(l[i] == std::string(1, 'a' + i - 16));
    }
    // 自行定义全部
    std::string s = "9876543210";
    BigNumberSetting::set_labels(s);
    l = BigNumberSetting::labels();
    CHECK(l.size() == 10);
    for(int i = 0; i < l.size(); i++) {
        CHECK(l[i] == std::string(1, s[i]));
    }
    // 用多个字符表示
    BigNumberSetting::init_labels();
    BigNumberSetting::set_labels(10, "10");
    bool is_single = BigNumberSetting::is_single_label();
    CHECK(is_single == false);
    // 取消多个字符
    BigNumberSetting::set_labels(10, "A");
    is_single = BigNumberSetting::is_single_label();
    CHECK(is_single == true);
    // 超出原本的范围
    BigNumberSetting::init_labels();
    BigNumberSetting::set_labels(46, "a"); // 37 - 45是""
    bool empty = BigNumberSetting::has_empty_or_same_label();
    CHECK(empty == true);
    // 在此基础上单个字符
    BigNumberSetting::set_labels(40, "b");
    is_single = BigNumberSetting::is_single_label();
    CHECK(is_single == true);
    // 重复字母
    BigNumberSetting::init_labels();
    BigNumberSetting::set_labels(11, "A");
    bool same = BigNumberSetting::has_empty_or_same_label();
    CHECK(same == true);
}

std::vector<long long> brute_multiply(const std::vector<long long>& a, const std::vector<long long>& b, long long MOD = 998244353) {
    size_t len_a = a.size();
    size_t len_b = b.size();
    std::vector<long long> result(len_a + len_b, 0);
    
    for (size_t i = 0; i < len_a; ++i) {
        long long carry = 0;
        for (size_t j = 0; j < len_b; ++j) {
            long long product = a[i] * b[j] + result[i + j] + carry;
            result[i + j] = product % MOD;
            carry = product / MOD;
        }
        size_t k = i + len_b;
        while (carry > 0) {
            long long sum = result[k] + carry;
            result[k] = sum % MOD;
            carry = sum / MOD;
            ++k;
        }
    }
    
    while (result.size() > 1 && result.back() == 0) {
        result.pop_back();
    }
    
    return result;
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
    init_gen();
    loop_check([&]() { return ntt_multiply_test();}, 10);
}

bool crt_multiply_test() {
    const long long l = 1000000; 
    int n = 100;
    std::vector<long long> a = rand_vector<long long>(1, n, [&](){return rand_int(l);});
    std::vector<long long> b = rand_vector<long long>(1, n, [&](){return rand_int(l);});
    std::vector<long long> c = CrtMultiplier<long long>::multiply(a, b, l);
    std::vector<long long> d = brute_multiply(a, b, l);
    if(c.size() != d.size()) return false;
    for (size_t i = 0; i < d.size(); i++) {
        if(c[i] != d[i]) return false;
    }
    return true;
}

void crt_multiply_test_small() {
    auto res1 = CrtMultiplier<>::multiply({321, 654, 987}, {789, 456, 123}, 1000);
    std::string res2 = "";
    for (int i = res1.size() - 1; i >= 0; i--) {
        res2 += std::to_string(res1[i]);
    }
    CHECK(res2 == "121932631112635269");    
}

void crt_multiply_test_int() {
    auto res1 = CrtMultiplier<int>::multiply({1234, 5}, {6789, 10}, 10000);
    std::string res2 = "";
    for (int i = res1.size() - 1; i >= 0; i--) {
        res2 += std::to_string(res1[i]);
    }
    CHECK(res2 == "5471227626");
}

void crt_square_test() {
    const long long l = 1000000; 
    int n = 100;
    std::vector<long long> a = rand_vector<long long>(1, n, [&](){return rand_int(l);});
    std::vector<long long> b = CrtMultiplier<long long>::multiply(a, a, l);
    std::vector<long long> c = CrtMultiplier<long long>::square(a, l);
    std::vector<long long> d = brute_multiply(a, a, l);
    CHECK(b.size() == d.size());
    CHECK(c.size() == d.size());
    for (size_t i = 0; i < d.size(); i++) {
        CHECK(b[i] == d[i]);
        CHECK(c[i] == d[i]);
    }
}

TEST_CASE("test crt", "[math][crt]") {
    init_gen();
    crt_multiply_test_small();
    crt_multiply_test_int();
    crt_square_test();
    loop_check([&]() { return crt_multiply_test();}, 10);
}

TEST_CASE("test big int compare", "[math][BigIntBase][compare]") {
    init_gen();
    BigIntBase a(0);
    BigIntBase b(0);
    BigIntBase c(1);
    BigIntBase d(-1);
    long long x = 1LL << 60;
    BigIntBase e(x);
    BigIntBase f(x + 1);
    BigIntBase g(x - 1);
    BigIntBase h(-x);

    CHECK(a == b);
    CHECK(a <= b);
    CHECK(a >= b);

    // 0和-0是相等的
    CHECK(a == 0);
    CHECK(a == 0LL);
    CHECK(0 == a);
    CHECK(a == -0);
    CHECK(-a == 0);
    CHECK(-a == -0);
    CHECK(+a == -a);

    CHECK(a < c);
    CHECK(a <= c);
    CHECK(a != c);

    CHECK(a > d);
    CHECK(a >= d);
    CHECK(a != d);

    // 确保不是用abs compare
    CHECK(c != d);
    CHECK(c > d);

    CHECK(+d == d);
    CHECK(-d == c);
    CHECK(d == -c);
    CHECK(-d == +c);

    CHECK(e < f);
    CHECK(e <= f);
    CHECK(e!= f);
    CHECK(e == x);
    CHECK(x == e);

    CHECK(e > g);
    CHECK(e >= g);
    CHECK(e!= g);

    CHECK(f > g);

    CHECK(e != h);
    CHECK(e == -h);
    CHECK(-e == h);
}

void big_int_div_mod_test_small() {
    BigIntBase a(7);
    BigIntBase b(3);
    auto c = a / b;
    CHECK(c == 2);
    auto d = a % b;
    CHECK(d == 1);    
}

void big_int_div_mod_test_simple_fast_small() {
    long long a = 1LL << 60;
    long long b = rand_int(-a, a);
    long long c = rand_int(-a, a);

    while (c == 0) c = rand_int(-a, a);

    // 使用simple_div
    fast_div_threshold = 1000;
    auto d = BigIntBase(b).div_mod(BigIntBase(c));
    CHECK(d.first() == b / c);
    CHECK(d.second() == b % c);

    // 使用fast_div
    fast_div_threshold = 0;
    auto e = BigIntBase(b).div_mod(BigIntBase(c));
    CHECK(e.first() == b / c);
    CHECK(e.second() == b % c);
}

TEST_CASE("test big int div mod", "[math][BigIntBase][div_mod]") {
    init_gen();
    big_int_div_mod_test_small();
    loop_check([&]() { big_int_div_mod_test_simple_fast_small(); return true;}, 10);
}

BigIntBase big_int_div_mod_test_benchmark() {
    BigIntBase a;
    auto& v = a.data_ref();
    v = rand_vector<unsigned int>(900, [&](){return rand_int(10000);});
    BigIntBase b(rand_int(3, 10000));
    return a.div_mod(b).first();
}

TEST_CASE("ensure simple div won't cost too long", "[math][BigIntBase][div_mod][!benchmark]") {
    init_gen();
    BENCHMARK("big div small") {
        return big_int_div_mod_test_benchmark();
    };

}