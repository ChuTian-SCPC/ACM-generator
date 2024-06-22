#include "../generator.h"
#include "../catch_main.hpp"

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

TEST_CASE("rand_even", "[rand][format]") {
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

TEST_CASE("rand_int_from_format", "[rand]") {
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
