#include "../generator.h"
#include "../catch_main.hpp"

using namespace generator::rand;

TEST_CASE("rand_int", "[rand]") {
    int run_times = 100;
    int x;
    long long xll;
    for (int i = 1; i <= run_times; i++) {
        x = rand_int(10);
        REQUIRE((x >= 0 && x < 10));
        
        x = rand_int(2, 10);
        REQUIRE((x >= 2 && x <= 10)); 
         
        xll = rand_int(1LL, 10);
        REQUIRE((xll >= 1 && xll <= 10));
        
        xll = rand_int(1e9 - 10, 1e9);
        REQUIRE((xll >= 1000000000 - 10 && xll <= 1000000000));
    }
}

TEST_CASE("format_int_range", "[rand]") {
    auto range = __format_to_int_range("[1,10]");
    REQUIRE((range.first == 1 && range.second == 10));
    range = __format_to_int_range("  [1,  10  ]");
    REQUIRE((range.first == 1 && range.second == 10));
    range = __format_to_int_range("(1, 10]");
    REQUIRE((range.first == 2 && range.second == 10));
    range = __format_to_int_range("[1, 10)");
    REQUIRE((range.first == 1 && range.second == 9));
    range = __format_to_int_range("[1, 1e9]");
    REQUIRE((range.first == 1 && range.second == 1000000000));
    range = __format_to_int_range("[-10, 1234.5]");
    REQUIRE((range.first == -10 && range.second == 1234));
}

TEST_CASE("rand_odd", "[rand]") {
    int run_times = 100;
    for (int i = 1; i <= run_times; i++) {
      int x = rand_odd(3);
      REQUIRE((x >= 1 && x <= 3 && x%2));
      x = rand_odd(4);
      REQUIRE((x >= 1 && x <= 3 && x%2));
      x = rand_odd(0, 3);
      REQUIRE((x >= 1 && x <= 3 && x%2)); 
      x = rand_odd(0, 2);
      REQUIRE(x == 1);
      x = rand_odd(-3, 0);
      REQUIRE((x >= -3 && x <= -1 && x%2));
      x = rand_odd(-4, 0);
      REQUIRE((x >= -3 && x <= -1 && x%2));
      x = rand_odd(-2, 0);
      REQUIRE(x == -1);
    }
}

TEST_CASE("rand_even", "[rand]") {
    int run_times = 100;
    for (int i = 1; i <= run_times; i++) {
      int x = rand_even(3);
      REQUIRE((x >= 0 && x <= 2 && x%2 == 0));
      x = rand_even(4);
      REQUIRE((x >= 0 && x <= 4 && x%2 == 0));
      x = rand_even(0, 3);
      REQUIRE((x >= 0 && x <= 2 && x%2 == 0)); 
      x = rand_even(1, 3);
      REQUIRE(x == 2);
      x = rand_even(-3, 0);
      REQUIRE((x >= -2 && x <= 0 && x%2 == 0));
      x = rand_even(-4, -1);
      REQUIRE((x >= -4 && x <= -2 && x%2 == 0));
      x = rand_even(-2, -1);
      REQUIRE(x == -2);
    }
}
