#include "../generator.h"
#include "../catch_main.hpp"

using namespace generator::rand;

TEST_CASE("rand_int", "[rand]") {
    int run_times = 1000;
    for (int i = 1; i <= run_times; i++) {
        int x = rand_int(10);
        REQUIRE((x >= 0 && x < 10));
    }
    for (int i = 1; i <= run_times; i++) {
        int x = rand_int(2, 10);
        REQUIRE((x >= 2 && x <= 10));
    }
    for (int i = 1; i <= run_times; i++) {
        auto x = rand_int(1LL, 10);
        REQUIRE((x >= 1 && x <= 10));
    }
}

