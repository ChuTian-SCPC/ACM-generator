#include "../generator.h"
#include "../catch_main.hpp"

using namespace std;
using namespace generator::rand;

TEST_CASE("rand_int", "[rand]") {
    int run_times = 100;
    int x;
    long long xll;
    for (int i = 1; i <= run_times; i++) {
        x = rand_int(10);
        CHECK((x >= 0 && x < 10));
        
        x = rand_int(2, 10);
        CHECK((x >= 2 && x <= 10)); 
         
        xll = rand_int(1LL, 10);
        CHECK((xll >= 1 && xll <= 10));
        
        xll = rand_int(1e9 - 10, 1e9);
        CHECK((xll >= 1000000000 - 10 && xll <= 1000000000));
    }
}

TEST_CASE("format_int_range", "[format]") {
    auto range = __format_to_int_range("[1,10]");
    CHECK((range.first == 1 && range.second == 10));
    range = __format_to_int_range("  [1,  10  ]");
    CHECK((range.first == 1 && range.second == 10));
    range = __format_to_int_range("(1, 10]");
    CHECK((range.first == 2 && range.second == 10));
    range = __format_to_int_range("[1, 10)");
    CHECK((range.first == 1 && range.second == 9));
    range = __format_to_int_range("[1, 1e9]");
    CHECK((range.first == 1 && range.second == 1000000000));
    range = __format_to_int_range("[-10, 1234.5]");
    CHECK((range.first == -10 && range.second == 1234));
}

TEST_CASE("rand_odd", "[rand]") {
    int run_times = 100;
    for (int i = 1; i <= run_times; i++) {
      int x = rand_odd(3);
      CHECK((x >= 1 && x <= 3 && x%2));
      x = rand_odd(4);
      CHECK((x >= 1 && x <= 3 && x%2));
      x = rand_odd(0, 3);
      CHECK((x >= 1 && x <= 3 && x%2)); 
      x = rand_odd(0, 2);
      CHECK(x == 1);
      x = rand_odd(-3, 0);
      CHECK((x >= -3 && x <= -1 && x%2));
      x = rand_odd(-4, 0);
      CHECK((x >= -3 && x <= -1 && x%2));
      x = rand_odd(-2, 0);
      CHECK(x == -1);
    }
}

TEST_CASE("rand_even", "[rand]") {
    int run_times = 100;
    for (int i = 1; i <= run_times; i++) {
      int x = rand_even(3);
      CHECK((x >= 0 && x <= 2 && x%2 == 0));
      x = rand_even(4);
      CHECK((x >= 0 && x <= 4 && x%2 == 0));
      x = rand_even(0, 3);
      CHECK((x >= 0 && x <= 2 && x%2 == 0)); 
      x = rand_even(1, 3);
      CHECK(x == 2);
      x = rand_even(-3, 0);
      CHECK((x >= -2 && x <= 0 && x%2 == 0));
      x = rand_even(-4, -1);
      CHECK((x >= -4 && x <= -2 && x%2 == 0));
      x = rand_even(-2, -1);
      CHECK(x == -2);
    }
}

TEST_CASE("rand_int_from_format", "[rand][format]") {
    int run_times = 100;
    int x;
    for (int i = 1; i <= run_times; i++) {
        x = rand_int("[-1, %d]", 1);
        CHECK((x >= -1 && x <= 1));
        x = rand_odd("[-1 , %s]", "1");
        CHECK((x >=-1 && x <= 1 && x%2));
        x = rand_even("(-1, 1.0]");
        CHECK(x == 0);
    }
}

TEST_CASE("format_double_range", "[format]") {
    auto range = __format_to_double_range("[1, 2]");
    CHECK((range.first ==  Approx(1.0) && range.second == Approx(2.1)));
    range = __format_to_double_range("(0.01, 1.23e1]");
    CHECK((range.first == Approx(0.011) && range.second == Approx(12.301)));
    range = __format_to_double_range("[10, 20]");
    CHECK((range.first == Approx(10.0) && range.second == Approx(20.1)));
    range = __format_to_double_range("(-1e-6, 1E-6)");
    CHECK((range.first == Approx(-0.0000009) && range.second == Approx(0.000001)));

}

TEST_CASE("rand_real", "[rand]") {
    int run_times = 100;
    double x;
    for (int i = 1; i <= run_times; i++) {
        x = rand_real();
        CHECK((x >= 0 && x < 1));
        x = rand_real(2);
        CHECK((x >= 0 && x < 2));
        x = rand_real(1, 2.0);
        CHECK((x >= 1 && x < 2));
    }    
}

TEST_CASE("rand_double_from_format", "[rand][format]") {
    int run_times = 100;
    double x;
    for (int i = 1; i <= run_times; i++) {
        x = rand_real("[1, 2)");
        CHECK((x >= 1.0 && x < 2.0));
        x = rand_real("[1, %s]", "2.00");
        CHECK((x >= 1.000 && x < 2.001));
        x = rand_real("(%d, 0.2e1)", 1);
        CHECK((x >= 1.1 && x < 2.0));
        x = rand_real("(-1e-6, 1E-2]");
        CHECK((x >= 0.0000009 && x < 0.0100001));
    }
}

TEST_CASE("rand_char", "[rand]") {
    int run_times = 100;
    char c;
    for (int i = 1; i <= run_times; i++) {
        c = rand_char();
        CHECK((c >= 'a' && c <= 'z'));
        c = rand_char(CharType::Number);
        CHECK((c >= '0' && c <= '9'));
        c = rand_char("[oash]");
        CHECK((c == 'o' || c == 'a' || c == 's' || c == 'h'));
    }  
}

TEST_CASE("rand_string", "[rand]") {
    int run_times = 100;
    string s;
    for (int i = 1; i <= run_times; i++) {
        s = rand_string(5);
        CHECK(s.length() == 5);
        for (auto c : s) {
            CHECK((c >= 'a' && c <= 'z'));
        }
        s = rand_string(3, 7, "[12%s]", "c");
        CHECK((s.length() >= 3 && s.length() <= 7));
        for (auto c : s) {
            CHECK((c == '1' || c == '2' || c == 'c'));
        }
    }
}

bool check_permutation(std::vector<int>& p, int start) {
    std::sort(p.begin(), p.end());
    if (p[0] != start)  return false;
    for (int i = 1; i < p.size(); i++) {
        if (p[i] != p[i - 1] + 1) {
            return false;
        }
    }
    return true;
}

TEST_CASE("rand_permutation", "[rand]") {
    auto p = rand_p(10);
    CHECK(check_permutation(p, 0));
    p = rand_p(20, 3);
    CHECK(check_permutation(p, 3));
}

bool check_sum(std::vector<int>& p, int sum) {
    int tot = 0;
    for (auto x : p) {
        tot += x;
    }
    return tot == sum;
}

TEST_CASE("rand_sum", "[rand]") {
    int run_times = 100;
    std::vector<int> p;
    for (int i = 1; i <= run_times; i++) {
        p = rand_sum(10, 100);
        CHECK(p.size() == 10);
        CHECK(check_sum(p, 100));

        p = rand_sum(20, 100, 3);
        CHECK(p.size() == 20);
        CHECK(check_sum(p, 100));
        for (auto x : p) {
            CHECK(x >= 3);
        }

        p = rand_sum(10, 100, 5, 12);
        CHECK(p.size() == 10);
        CHECK(check_sum(p, 100));
        for (auto x : p) {
            CHECK((x >= 5 && x <= 12));
        }        
    }
}
