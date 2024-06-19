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

TEST_CASE("format_range", "[rand]") {
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

