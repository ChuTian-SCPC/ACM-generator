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