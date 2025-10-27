#pragma once
#include "test_basic.hpp"
#include "algorithm/Manacher.h"
using namespace generator::all;

TEST_CASE("rand sum", "[rand_array][rand_sum]") {
    init_gen();
    std::vector<long long> v = rand_sum(100, 1e10, -1e9, 1e9);
    CHECK(v.size() == 100);
    long long sum = 0;
    for (auto& x : v) {
        sum += x;
        CHECK((-1e9 <= x && x <= 1e9));
    }
    CHECK(sum == 1e10);
}

bool palindrome_check() {
    int n = rand_int(1, 1e5);
    int p = rand_int(1, n);
    auto s = rand_palindrome(n, p);
    if (s.size() != n) return false;
    OY::Manacher<true, true> m(s);
    int max_p = 0;
    for (int i = 0; i < n; i++) {
        max_p = std::max(max_p, (int)m.query_max_odd_arm(i) * 2 + 1);
        if (i + 1 < n) max_p = std::max(max_p, (int)m.query_max_even_arm(i, i + 1) * 2);
    }
    return max_p >= p;
}

TEST_CASE("rand palindrome", "[rand_array][rand_palindrome]") {
    init_gen();
    bool f = loop_check([&]() { return palindrome_check();}, 10);
    CHECK(f);
}

bool bracket_seq_check() {
    int n = rand_even(1, 1e5);
    std::vector<std::string> bracket_seqs = {"()", "[]", "{}"};
    int seqs_count = rand_int(1, 3);
    std::string barcket = "";
    for (int i = 0; i < seqs_count; i++) barcket += bracket_seqs[i];
    auto s = rand_bracket_seq(n, barcket);
    if (s.size() != n) return false;
    std::stack<char> st;
    for (auto& c : s) {
        bool match = false;
        for (int i = 0; i < seqs_count; i++) {
            if (c == bracket_seqs[i][0]) {
                st.push(bracket_seqs[i][1]);
                match = true;
            }
            else if (c == bracket_seqs[i][1]) {
                if (st.empty() || st.top() != c) return false;
                st.pop();
                match = true;
            }
        }
        if (!match) return false;
    }
    if (!st.empty()) return false;
    return true;
}

TEST_CASE("rand bracket sequence", "[rand_array][rand_bracket_seq]") {
    init_gen();
    bool f = loop_check([]() { return bracket_seq_check(); }, 10);
    CHECK(f);
}

bool shuffle_index_check() {
    int n = rand_int(1, 1e3);
    int sum = rand_int(1, 1e5);
    std::vector<int> v = rand_sum(n, sum);
    int offset = rand_int(1, 10);
    std::vector<int> count(n);
    std::vector<int> index = shuffle_index(v, offset);
    for (auto& i : index) {
        int p = i - offset;
        if (p < 0 || p >= n) return false;
        count[p]++;
    }
    for (int i = 0; i < n; i++) {
        if (count[i]!= v[i]) return false;
    }
    return true;
}

TEST_CASE("shuffle vector index", "[rand_array][shuffle_index]") {
    init_gen();
    bool f = loop_check([]() { return shuffle_index_check(); }, 10);
    CHECK(f);
}

template <typename T>
std::vector<T> rand_vector_origin(int size, std::function<T()> func) {
    __judge_vector_lower_bound(size, "vector");
    __judge_vector_upper_bound(size, "vector");
    OutStream vector_stream(false);
    _defl.swap(vector_stream);    
    std::vector<T> v;
    for(int i = 0; i < size; i++){
        T x = func();
        v.emplace_back(x);
    }
    _defl.swap(vector_stream);
    return v;
}

TEST_CASE("rand vector speed up", "[rand_array][rand_vector][!benchmark]") {
    init_gen();
    BENCHMARK("rand vector origin") {
        return rand_vector_origin<int>(10000000, [](){return rand_int(-1000000, 1000000);});
    };
    BENCHMARK("rand vector") {
        return rand_vector<int>(10000000, [](){return rand_int(-1000000, 1000000);});
    };
}

TEST_CASE("rand range query", "[rand_array][rand_range][rand_range_query]") {
    init_gen();
    OutStream vector_stream(false);
    _defl.swap(vector_stream); 
    bool f1 = loop_check([]() {
        auto x = rand_range_query(10, 1, 100);
        for (auto p : x) {
            if (p.first < 1 || p.first > 100 || p.second < 1 || p.second > 100 || p.first > p.second) return false;
        }
        return true;
    }, 100);
    CHECK(f1);

    bool f2 = loop_check([]() {
        auto x = rand_range_query(10, 1, 100, 100, 100);
        int sum = 0;
        for (auto p : x) {
            if (p.first < 1 || p.first > 100 || p.second < 1 || p.second > 100 || p.first > p.second) return false;
            sum += p.second - p.first + 1;
        }
        return sum == 100;
    }, 100);
    CHECK(f2);

    bool f3 = loop_check([]() {
        auto x = rand_range_query(10, 1, 100, 100);
        int sum = 0;
        for (auto p : x) {
            if (p.first < 1 || p.first > 100 || p.second < 1 || p.second > 100 || p.first > p.second) return false;
            sum += p.second - p.first + 1;
        }
        return sum >= 100;
    }, 100);
    CHECK(f3);

    bool f4 = loop_check([]() {
        auto x = rand_range_query(100000, 1, 1e6, 1e9);
        long long sum = 0;
        for (auto p : x) {
            if (p.first < 1 || p.first > (int)1e6 || p.second < 1 || p.second > (int)1e6 || p.first > p.second) return false;
            sum += p.second - p.first + 1;
        }
        return sum >= (int)1e9;
    }, 10);
    CHECK(f4);
    _defl.swap(vector_stream);
}

TEST_CASE("rand int vector ordered and distinct", "[rand_array][rand_vector][VectorOrder][VectorUniqueness]") {
    init_gen();
    bool f1 = loop_check([]() {
        int l = rand_int(100, 200);
        auto x = rand_vector(100, 1, l, VectorOrder::Ascending, VectorUniqueness::Distinct);
        std::vector<int> y(l + 1, 0);
        for (int i = 0; i < 100; i++) {
            y[x[i]]++;
            if (y[x[i]] > 1) return false;
            if (i > 0 && x[i] <= x[i - 1]) return false;
        }
        return true;
    }, 10);
    CHECK(f1);

    bool f2 = loop_check([]() {
        int l = rand_int(100, 200);
        auto x = rand_vector(100, 1, l, VectorOrder::Descending, VectorUniqueness::Distinct);
        std::vector<int> y(l + 1, 0);
        for (int i = 0; i < 100; i++) {
            y[x[i]]++;
            if (y[x[i]] > 1) return false;
            if (i > 0 && x[i] >= x[i - 1]) return false;
        }
        return true;
    }, 10);
    CHECK(f2);

    bool f3 = loop_check([]() {
        int l = rand_int(100, 200);
        auto x = rand_vector(100, 1, l, VectorOrder::Ascending);
        for (int i = 0; i < 100; i++) {
            if (i > 0 && x[i] < x[i - 1]) return false;
        }
        return true;
    }, 10);
    CHECK(f3);

    bool f4 = loop_check([]() {
        int l = rand_int(100, 200);
        auto x = rand_vector(100, 1, l, VectorOrder::Descending);
        for (int i = 0; i < 100; i++) {
            if (i > 0 && x[i] > x[i - 1]) return false;
        }
        return true;
    }, 10);
    CHECK(f4);

    bool f5 = loop_check([]() {
        int l = rand_int(100, 200);
        auto x = rand_vector(100, 1, l, VectorOrder::Random, VectorUniqueness::Distinct);
        std::vector<int> y(l + 1, 0);
        for (int i = 0; i < 100; i++) {
            y[x[i]]++;
            if (y[x[i]] > 1) return false;
        }
        return true;
    }, 10);
    CHECK(f5);
}